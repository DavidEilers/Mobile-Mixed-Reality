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

#define BUFFER_SIZE 1024

using namespace std;

enum MessageType {
    MSG_STRING,
    MSG_INTS,
    MSG_FLOATS,
};

/**
 * default message type for sending strings
 */
class BaseMessage {
public:
    static const MessageType TYPE = MSG_STRING;

    string payload;

    /**
     * constructor
     */
    BaseMessage() {

    }

    /**
     * converts the message to bytes to be sent over the network
     * @param buffer buffer of size BUFFERSIZE to store the message in
     * @return returns true size of used buffer (<= BUFFERSIZE)
     */
    virtual int to_bytes(char *buffer) {
        buffer[0] = TYPE;
        for (int i = 0; i < payload.size(); i++) {
            buffer[1 + i] = payload.at(i);
        }
        //making sure one can easily find the end of the payload
        buffer[payload.size()] = '\0';

        return payload.size() + 1;
    }

    /**
     * creates BaseMessage object containing the payload string from buffer if
     * the buffer contains suitable data
     * @param buffer buffer to create the message object from
     * @param msg_obj pointer to store the message object
     * @return returns true if the buffer contains valid data of the right type, false otherwise
     */
    static bool from_bytes(char *buffer, BaseMessage *msg_obj) {
        MessageType content_type = (MessageType) buffer[0];
        if (content_type != TYPE) return false;
        msg_obj->payload.assign(buffer + 1);
        return true;
    }
};

/**
 * struct for storing messages
 * contains TYPE to identify the contents of the payload
 * contains LENGTH of payload. 0 -> no payload
 */

struct Message {
    u_int16_t TYPE;
    u_int16_t LENGTH;
    char *payload;

    ~Message() {
        delete payload;
    }
};

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
     */
    static void send_message(BaseMessage &msg_obj, string addr, int port);

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
