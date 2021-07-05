#include <string>

#ifndef TEAMPRAKTIKUM_MESSAGES_HPP
#define TEAMPRAKTIKUM_MESSAGES_HPP

using namespace std;

enum MessageType {
    MSG_BASE = 0,
    MSG_STRING = 1,
    MSG_INTS = 2,
    MSG_FLOATS = 3,
    MSG_CONNECT = 4,
    MSG_ERR_NAME_IN_USE = 5,
    MSG_LEAVE = 6,
    MSG_ERR_GAME_FULL = 7
};
// TYPE > 7 can be used for own custom messages

/**
 * default message type for sending strings
 */
class BaseMessage {
public:
    static const char TYPE = MSG_BASE;

    /**
     * constructor
     */
    BaseMessage() {

    }

    int get_bytes(char* buffer){
        return to_bytes(buffer);
    }

    /**
     * converts the message to bytes to be sent over the network
     * @param buffer buffer of size BUFFERSIZE to store the message in
     * @return returns true size of used buffer (<= BUFFERSIZE)
     */
    virtual int to_bytes(char *buffer) {
        buffer[0] = TYPE;
        return 1;
    }

    /**
     * creates BaseMessage object if the buffer contains suitable data
     * @param buffer buffer to create the message object from
     * @param msg_obj pointer to store the message object
     * @return returns true if the buffer contains valid data of the right type, false otherwise
     */
    virtual bool from_bytes(char *buffer) {
        return buffer[0] == TYPE;
    }
};

/**
 * message type for sending strings
 */
class StringMessage : public BaseMessage {
public:
    const char TYPE = MSG_STRING;

    string payload;

    /**
     * converts the message to bytes to be sent over the network
     * @param buffer buffer of size BUFFERSIZE to store the message in
     * @return returns size of used buffer (<= BUFFERSIZE)
     */
    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        for (int i = 0; i < payload.size(); i++) {
            buffer[1 + i] = payload.at(i);
        }
        //making sure one can easily find the end of the player_name
        buffer[payload.size()] = '\0';

        return payload.size() + 1;
    }

    /**
     * creates StringMessage object containing the payload string from buffer if
     * the buffer contains suitable data
     * @param buffer buffer to create the message object from
     * @return returns true if the buffer contains valid data of the right type, false otherwise
     */
    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->payload.assign(buffer + 1);
        return true;
    }
};

/**
 * connect message object to join games
 * carries a player name and port to reach slave
 * slave sends this to master to connect
 * on success master answers too with an connect message object and his port and player name
 */
class ConnectMessage : public BaseMessage {
public:
    const char TYPE = MSG_CONNECT;

    string player_name;
    u_int16_t port;

    /**
     * converts the message to bytes to be sent over the network
     * @param buffer buffer of size BUFFERSIZE to store the message in
     * @return returns count of used bytes (<= BUFFERSIZE)
     */
    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;

        int s = sizeof(u_int16_t);

        u_int16_t *ptr = (u_int16_t *) (buffer + 1);
        ptr[0] = port;

        for (int i = 0; i < player_name.size(); i++) {
            buffer[1 + s + i] = player_name.at(i);
        }
        //making sure one can easily find the end of the player_name
        buffer[3 + player_name.size()] = '\0';

        return 1 + s + player_name.size();
    }

    /**
     * creates StringMessage object containing the player_name string from buffer if
     * the buffer contains suitable data
     * @param buffer buffer to create the message object from
     * @return returns true if the buffer contains valid data of the right type, false otherwise
     */
    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;

        int s = sizeof(u_int16_t);

        //get one u_int16_t with one byte offset
        this->port = ((u_int16_t *) (buffer + 1))[0];
        //following bytes are player name
        this->player_name.assign(buffer + 1 + s);
        return true;
    }
};


/**
 * error message sent by a master following a connect by a slave
 * if the transmitted player_name is already in use
 */
class ErrorNameInUseMessage : public BaseMessage {
public:
    const MessageType TYPE = MSG_ERR_NAME_IN_USE;
};


/**
 * error message sent by master to the connecting slave
 * if the maximum number of connected slaves is already reached
 */
class ErrorGameFullMessage : public BaseMessage {
public:
    const MessageType TYPE = MSG_ERR_GAME_FULL;
};


/**
 * leave message can be sent by slave to disconnect from master
 * or by master to disconnect a slave
 * if leave message is broadcast by master all slaves disconnect -> could be used to close a game
 * contains player name of leaving player
 */
class LeaveMessage : public BaseMessage {
public:
    const char TYPE = MSG_LEAVE;
    string player_name;

    /**
     * converts the message to bytes to be sent over the network
     * @param buffer buffer of size BUFFERSIZE to store the message in
     * @return returns size of used buffer (<= BUFFERSIZE)
     */
    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        for (int i = 0; i < player_name.size(); i++) {
            buffer[1 + i] = player_name.at(i);
        }
        //making sure one can easily find the end of the player_name
        buffer[player_name.size()] = '\0';

        return player_name.size() + 1;
    }

    /**
     * creates LeaveMessage object containing the player_name string from buffer if
     * the buffer contains suitable data
     * @param buffer buffer to create the message object from
     * @return returns true if the buffer contains valid data, false otherwise
     */
    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->player_name.assign(buffer + 1);
        return true;
    }
};

#endif //TEAMPRAKTIKUM_MESSAGES_HPP