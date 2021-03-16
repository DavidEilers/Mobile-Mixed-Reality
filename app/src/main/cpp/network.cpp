//
// Created by Oskar on 15.01.2021.
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

#include "Network.h"

Server::Server(int listening_port) {
    PORT = listening_port;
    listening = false;
    has_msg = false;
}

void Server::start_listening() {
    if (!listening.load()) {
        listening = true;
        listening_thread = thread([this] { listening_function(); });
    }
}

void Server::stop_listening() {
    if (listening.load()) {
        listening = false;
        listening_thread.join();
    }
}

bool Server::is_listening() {
    return listening.load();
}

bool Server::has_messages() {
    return has_msg.load();
}

void Server::listening_function() {

    __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Listening thread started on port: %d.",
                        PORT);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addr_len = sizeof(address);

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int err = 0;

    //create socket objekt

    err = server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket creation error! Error: %d",
                            err);
    }

    //set socket options => use address and port even if they are still blocked by a previous instance of this program

    err = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d",
                            err);
    }

    //set socket options => receive timeout

    err = setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d",
                            err);
    }

    //set socket options => send timeout

    err = setsockopt(server_fd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "Socket option error! Error: %d",
                            err);
    }

    //create address object

    //IPv4
    address.sin_family = AF_INET;
    //Any Address is allowed to connect to us
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /*
     * There is another function called std::bind. Because I am using the namespace std
     * but I want to use the socket bind function I use ::bind.
     */

    //bind to address

    err = ::bind(server_fd, (struct sockaddr *) &address, addr_len);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket",
                            "could not bind to address. err: %d", err);
    }

    //start listening for TCP connections

    err = listen(server_fd, 3);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "listen err: %d", err);
    }

    while (listening.load()) {

        //address is used again but as incoming address
        new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addr_len);

        if (new_socket > 0) { // new_socket <= 0 -> timeout
            __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Connection accepted");

            struct RawContainer container;

            //max message length = 1024
            container.buffer = new char[BUFFER_SIZE];
            read(new_socket, container.buffer, BUFFER_SIZE);

            string ip_string = get_ip_string(address);

            container.from_addr = ip_string;

            store_message(container);

            close(new_socket);

        } else {
            __android_log_print(ANDROID_LOG_DEBUG, "server new_socket value", "%d <= 0",
                                new_socket);
        }
    }

    close(server_fd);

    __android_log_print(ANDROID_LOG_DEBUG, "server socket", "Listening thread finished.");
}

string Server::get_ip_string(struct sockaddr_in addr) {

    char ip[INET6_ADDRSTRLEN];

    //PIv4
    if (addr.sin_family == AF_INET) {
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    }

    string ip_string(ip);

    return ip_string;
}

void Server::store_message(struct RawContainer con) {
    msg_vector_mutex.lock();
    messages.push_back(con);
    msg_vector_mutex.unlock();

    has_msg = true;
}

vector<struct RawContainer> Server::get_messages() {
    msg_vector_mutex.lock();
    vector<struct RawContainer> copy(messages);
    messages.clear();
    msg_vector_mutex.unlock();

    has_msg = false;

    return copy;
}

void Server::send_message(BaseMessage &msg_obj, string addr, int port) {

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending started.");

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int err = 0;

    //create socket object

    err = sock = socket(AF_INET, SOCK_STREAM, 0);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket creation error! Error: %d",
                            err);

        return;
    }

    //set options for socket => receive timeout

    err = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket option error! Error: %d",
                            err);

        return;
    }

    //set options for socket => send timeout

    err = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Socket option error! Error: %d",
                            err);

        return;
    }

    //create address struct

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // convert IPv4 and IPv6 address strings to binary form

    err = inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr);
    if (err <= 0) {
        __android_log_print(ANDROID_LOG_ERROR, "client socket",
                            "Invalid address / Address not supported! Error: %d", err);

        return;
    }

    //connect to address

    err = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "client socket", "Connection failed! Error: %d",
                            err);

        return;
    }

    int size = msg_obj.to_bytes(buffer);

    send(sock, buffer, size, 0);

    close(sock);

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending finished.");

    return;
}
