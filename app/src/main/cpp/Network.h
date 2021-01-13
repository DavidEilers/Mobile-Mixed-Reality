//
// Created by Oskar on 11.01.2021.
//

#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <atomic>
#include <vector>
#include <android/log.h>
#include <errno.h>

#ifndef TEAMPRAKTIKUM_NETWORK_H
#define TEAMPRAKTIKUM_NETWORK_H

using namespace std;

/**
 * Struct for storing the text and sender address of a message at once.
 */
struct message{
    string text;
    string from_addr;
};

class Server {
public:
    void startListening();
    void stopListening();
    bool isListening();
    bool hasMessages();
    vector<struct message> getMessages();
    Server(int listening_port);
private:
    int PORT;
    mutex msg_vector_mutex;
    atomic<bool> listening, has_msg;
    thread listening_thread;

    vector<struct message> messages;

    string getIPString(struct sockaddr_in addr);
    void storeMessage(struct message msg);
    void listeningFunction();
};

/**
 * Constructor
 * @param listening_port - port to listen on
 */
Server::Server(int listening_port) {
    PORT = listening_port;
    listening = false;
    has_msg = false;
}

/**
 * Start the listening thread.
 */
void Server::startListening() {
    if(!listening.load()) {
        listening = true;
        listening_thread = thread([this]{listeningFunction();});
    }
}

/**
 * Stop the listening thread.
 */
void Server::stopListening() {
    if(listening.load()) {
        listening = false;
        listening_thread.join();
    }
}

/**
 * Returns true if the server is listening and false if not.
 * @return true if the listening thread is running, false otherwise.
 */
bool Server::isListening() {
    return listening.load();
}

/**
 * Returns true if the server has stored messages.
 * @return vector containing all messages since the last call of this function.
 */
bool Server::hasMessages() {
    return has_msg.load();
}

/**
 * Listens on the given port for connections and stores received messages.
 * This function is called as thread from the public function startListening().
 */
void Server::listeningFunction() {

    __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Listening thread started on port: %d.", PORT);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);
    char buffer[1024] = {0};

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int err = 0;

    err = server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket creation error! Error: %d", err);
    }

    err = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d", err);
    }

    err = setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if (err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d", err);
    }

    err = setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
    if (err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d", err);
    }

    //IPv4
    address.sin_family = AF_INET;
    //Any Address is allowed to connect to us
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /*
     * There is another function called std::bind. Because I am using the namespace std
     * but I want to use the socket bind function I use ::bind.
     */
    err = ::bind(server_fd, (struct sockaddr * ) &address, addr_len);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "could not bind to address. err: %d", err);
    }

    err = listen(server_fd, 3);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "listen err: %d", err);
    }

    while(listening.load()){

        //address is used again but as incoming address
        new_socket = accept(server_fd, (struct sockaddr * ) &address, (socklen_t*) &addr_len);

        if (new_socket > 0) { // new_socket <= 0 -> timeout
            __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Connection accepted");

            //Max message length = 1024
            read(new_socket, buffer, 1024);

            string ip_string = getIPString(address);

            string text(buffer);

            struct message msg;
            msg.from_addr = ip_string;
            msg.text = text;

            storeMessage(msg);

            //Echo
            send(new_socket, buffer, strlen(buffer), 0);

            close(new_socket);

        } else {
            __android_log_print(ANDROID_LOG_DEBUG, "server new socket value", "%d <= 0", new_socket);
        }
    }

    close(server_fd);

    __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Listening thread finished.");
}

/**
 * Get the IP address out of the socket address struct.
 * @param addr socket address struct
 * @return the IP address extracted from the struct as string
 */
string Server::getIPString(struct sockaddr_in addr) {

    char ip[INET6_ADDRSTRLEN];

    //PIv4
    if(addr.sin_family == AF_INET){
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    }

    string ip_string(ip);

    return ip_string;
}

/**
 * Stores messages thread safe inside the messages vector.
 * @param msg message struct to store
 */
void Server::storeMessage(struct message msg) {
    msg_vector_mutex.lock();
    messages.push_back(msg);
    msg_vector_mutex.unlock();

    has_msg = true;
}

/**
 * Returns vector with all received messages since the last call of this function.
 * @return vector containing the messages since last call.
 */
vector<struct message> Server::getMessages() {
    msg_vector_mutex.lock();
    vector<struct message> copy(messages);
    messages.clear();
    msg_vector_mutex.unlock();

    has_msg = false;

    return copy;
}


/*
 * Helpful network functions not associated with the Server class that are
 * not part of the Server class because they do not make use of any member variables or functions.
 * This way they can be started as thread more easily.
 */

/**
 * Sends a string over the network using TCP to the given address and port.
 * Blocks until finished.
 * If no connection can be established the timeout is one second.
 * @param msg string to send
 * @param addr target IP address string, for example: "192.168.0.12"
 * @param port target port
 */
void sendString(string msg, string addr, int port) {

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending started.");

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int err = 0;

    err = sock = socket(AF_INET, SOCK_STREAM, 0);
    if(err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket creation error! Error: %d", err);

        return;
    }

    err = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    if (err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket option error! Error: %d", err);

        return;
    }

    err = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
    if (err < 0){
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket option error! Error: %d", err);

        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    err = inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr);
    if(err <= 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Invalid address / Address not supported! Error: %d", err);

        return;
    }

    err = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (err < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Connection failed! Error: %d", err);

        return;
    }

    send(sock , msg.c_str() , msg.length() , 0 );

    close(sock);

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending finished.");

    return;
}

#endif //TEAMPRAKTIKUM_NETWORK_H
