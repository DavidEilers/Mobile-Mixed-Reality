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

class Server {
public:
    /**
     * Constructor
     * @param listening_port - port to listen on
     */
    Server(int listening_port);

    /**
     * Start the listening thread.
     */
    void start_listening();

    /**
     * Stop the listening thread.
     */
    void stop_listening();

    /**
     * Returns true if the server is listening and false if not.
     * @return true if the listening thread is running, false otherwise.
     */
    bool is_listening();

    /**
     * Returns true if the server has stored messages.
     * @return vector containing all messages since the last call of this function.
     */
    bool has_messages();

    /**
     * Returns vector with all received messages since the last call of this function.
     * @return vector containing the messages since last call.
     */
    vector<struct RawContainer> get_messages();

    /**
     * sends the given message to the given address and port
     * @param message message to send
     * @param addr target address
     * @param port target port
     * @return returns true if sending was successful, false otherwise
     */
    static bool send_message(BaseMessage &msg_obj, string addr, int port);

private:
    int PORT;
    mutex msg_vector_mutex;
    atomic<bool> listening, has_msg;
    thread listening_thread;

    vector<struct RawContainer> messages;

    /**
     * Get the IP address out of the socket address struct.
     * @param addr socket address struct
     * @return the IP address extracted from the struct as string
     */
    string get_ip_string(struct sockaddr_in addr);

    /**
     * Stores messages thread safe inside the messages vector.
     * @param msg message struct to store
     */
    void store_message(struct RawContainer con);

    /**
     * Listens on the given port for connections and stores received messages.
     * This function is called as thread from the public function start_listening().
     */
    void listening_function();
};

#endif //TEAMPRAKTIKUM_NETWORK_H
