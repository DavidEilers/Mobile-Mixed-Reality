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

#include "Messages.hpp"

#ifndef TEAMPRAKTIKUM_NETWORK_H
#define TEAMPRAKTIKUM_NETWORK_H

#define BUFFER_SIZE 1024

using namespace std;

/**
 * Struct for storing buffer and the source ip address
 */
struct RawContainer {
    char *buffer;
    string from_addr;
};

struct MessageData {
    BaseMessage *msg;
    string addr;
    int port;
};

/**
 * Interface for Master and Slaves to receive Messages
 */
class IMessageListener {
public:
    virtual void on_handle_message(RawContainer container) = 0;
};

class Server {
public:
    /**
     * Constructor
     * @param listening_port - port to listen on
     */
    Server(int listening_port);

    /**
     * Start the running and sending threads.
     */
    void start();

    /**
     * Stop the running and sending threads.
     */
    void stop();

    /**
     * Returns true if the server is running and false if not.
     * @return true if the running thread is running, false otherwise.
     */
    bool is_running();

    /**
     * sends the given message to the given address and port
     * @param message message to send
     * @param addr target address
     * @param port target port
     * @return returns true if sending was successful, false otherwise
     */
    void send_message(BaseMessage *msg_obj, string addr, int port);

    /**
     *
     */
    void add_message_listener(IMessageListener *listener);

private:
    int PORT;
    mutex msg_vector_mutex;
    atomic<bool> running, has_msg;
    thread listening_thread, sending_thread;

    vector<IMessageListener *> listeners;
    vector<struct MessageData> messages;

    /**
     * Get the IP address out of the socket address struct.
     * @param addr socket address struct
     * @return the IP address extracted from the struct as string
     */
    string get_ip_string(struct sockaddr_in addr);

    /**
     * Listens on the given port for connections and stores received messages.
     * This function is called as thread from the public function start().
     */
    void listening_function();

    /**
     * Sends all messages to the given target. Runs as own thread.
     */
    void sending_function();

    /**
     * Sends the actual data to the given ip address and port
     * @param buffer data to send
     * @param size size of buffer
     * @param ip_str ip address as string
     * @param port port of target
     */
    void send_data(char *buffer, int size, std::string addr, int port);
};

#endif //TEAMPRAKTIKUM_NETWORK_H
