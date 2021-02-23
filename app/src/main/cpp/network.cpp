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
    char buffer[BUFFER_SIZE] = {0};

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

            //max message length = 1024
            read(new_socket, buffer, BUFFER_SIZE);

            string ip_string = get_ip_string(address);

            struct Message msg;

            //first two bytes in message
            msg.TYPE = ((u_int16_t *) buffer)[0];

            //next two bytes
            msg.LENGTH = ((u_int16_t *) buffer)[1];

            __android_log_print(ANDROID_LOG_DEBUG, "MESSAGE CONTAINER", "TYPE = %d, LENGTH = %d",
                                msg.TYPE, msg.LENGTH);
            __android_log_print(ANDROID_LOG_DEBUG, "MESSAGE CONTAINER", "TYPE = %x, LENGTH = %x",
                                msg.TYPE, msg.LENGTH);

            //rest of the buffer is the payload

            if (msg.LENGTH == 0) {
                msg.payload = 0;
            } else {
                char *payload = new char[msg.LENGTH + 1];

                for (int i = 0; i < msg.LENGTH; i++) {
                    payload[i] = buffer[4 + i];
                    __android_log_print(ANDROID_LOG_DEBUG, "PAYLOAD", "%d", payload[i]);
                }

                __android_log_print(ANDROID_LOG_DEBUG, "PAYLOAD", "%s", payload);

                msg.payload = payload;

            }

            struct MessageContainer container;

            container.from_addr = ip_string;
            container.message = &msg;

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

void Server::store_message(struct MessageContainer con) {
    msg_vector_mutex.lock();
    messages.push_back(con);
    msg_vector_mutex.unlock();

    has_msg = true;
}

vector<struct MessageContainer> Server::get_messages() {
    msg_vector_mutex.lock();
    vector<struct MessageContainer> copy(messages);
    messages.clear();
    msg_vector_mutex.unlock();

    has_msg = false;

    return copy;
}

void Server::send_string(u_int16_t type, string msg, string addr, int port) {

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

    //first two bytes = message type
    char *as_char = (char *) &type;
    buffer[0] = as_char[0];
    buffer[1] = as_char[1];

    //next two bytes = payload length
    u_int16_t msg_len = msg.length();
    as_char = (char *) &msg_len;
    buffer[2] = as_char[0];
    buffer[3] = as_char[1];

    //after that =>
    const char *tmp = msg.c_str();
    for (int i = 0; i < msg_len; i++) {
        buffer[4 + i] = tmp[i];
    }
    send(sock, buffer, msg.length() + 4, 0);

    close(sock);

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending finished.");

    return;
}

void Server::send_message(struct Message message, string addr, int port) {

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

    //first two bytes = message type
    char *as_char = (char *) (&message.TYPE);
    buffer[0] = as_char[0];
    buffer[1] = as_char[1];

    //next two bytes = payload length
    as_char = (char *) (&message.LENGTH);
    buffer[2] = as_char[0];
    buffer[3] = as_char[1];

    //after that => payload
    for (int i = 0; i < message.LENGTH; i++) {
        buffer[4 + i] = message.payload[i];
    }

    send(sock, buffer, message.LENGTH + 4, 0);

    close(sock);

    __android_log_print(ANDROID_LOG_DEBUG, "client socket", "Sending finished.");

    return;
}
