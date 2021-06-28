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
#include <chrono>

#include "Network.h"

Server::Server(int listening_port) {
    PORT = listening_port;
    running = false;
    has_msg = false;
}

void Server::start() {
    if (!running.load()) {
        running = true;
        listening_thread = thread([this] { listening_function(); });
        sending_thread = thread([this] { sending_function(); });
    }
}

void Server::stop() {
    if (running.load()) {
        running = false;
        listening_thread.join();
    }
}

bool Server::is_running() {
    return running.load();
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

    //start running for TCP connections

    err = listen(server_fd, 3);

    if (err < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "server socket", "listen err: %d", err);
    }

    while (running.load()) {

        do {
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

                for (IMessageListener *listener: listeners) {
                    listener->on_handle_message(container);
                }
                __android_log_print(ANDROID_LOG_VERBOSE, "server", "received message");

                delete[] container.buffer;

                close(new_socket);

            } else {
            __android_log_print(ANDROID_LOG_DEBUG, "server new_socket value", "%d <= 0",
                                errno);

            __android_log_print(ANDROID_LOG_DEBUG, "server new_socket value", "%s",
                                strerror(errno));
        }
        }while(new_socket==-1&&errno==11);
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

void Server::send_data(char *buffer, int size, string addr, int port) {

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending started.");

    int sock = 0;
    struct sockaddr_in serv_addr;

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
    __android_log_print(ANDROID_LOG_VERBOSE, "network send", "%d", buffer[0]);

    send(sock, buffer, size, 0);

    close(sock);

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending finished.");
}

void Server::add_message_listener(IMessageListener *listener) {
    listeners.push_back(listener);
}

void Server::sending_function() {
    while (running.load()) {
        if (messages.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            msg_vector_mutex.lock();

            MessageData msg_data = messages[0];
            messages.erase(messages.begin());

            char *buffer = new char[BUFFER_SIZE];
            int size = msg_data.msg->to_bytes(buffer);

            send_data(buffer, size, msg_data.addr, msg_data.port);

            delete[] buffer;

            msg_vector_mutex.unlock();
        }
    }
}

void Server::send_message(BaseMessage *msg_obj, string addr, int port) {
    msg_vector_mutex.lock();

    struct MessageData msg_data;

    msg_data.msg = msg_obj;
    msg_data.addr = addr;
    msg_data.port = port;

    messages.push_back(msg_data);

    msg_vector_mutex.unlock();
}
