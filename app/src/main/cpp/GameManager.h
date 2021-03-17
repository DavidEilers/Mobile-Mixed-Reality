//
// Created by Oskar on 22.02.2021.
//

#ifndef TEAMPRAKTIKUM_GAMEMANAGER_H
#define TEAMPRAKTIKUM_GAMEMANAGER_H

#include <vector>

#include "Network.h"

/**
 * struct to hold information about a connected slave
 * contains:
 * name (string) -> player name
 * ip (string) -> ip to reach slave
 * port (int) -> port to reach slave
 */

struct ConnectedSlave {
    std::string name;
    std::string ip;
    int port;
};

/**
 * master class represents the master of communication
 * slaves can connect or disconnect to it
 * the master can send direct messages or broadcasts to slaves
 */

class Master {
public:
    /**
     * constructor
     * @param player_name name of the master player
     * @param port port on which the game is hosted
     * @param max_slaves_count limits how many slaves can connect to the master
     */
    Master(std::string player_name, int port, int max_slaves_count);

    /**
     * runs one iteration of the master
     * call in game loop
     * handles connecting and leaving and calls handle messages with new messages
     */
    void tick();

    /**
     * @return returns a vector containing all connected slaves
     */
    std::vector<struct ConnectedSlave> get_connected_slaves();

    /**
     * broadcasts a given message to all connected slaves
     * @param message message to send to all slaves
     */
    void broadcast(BaseMessage message);

    /**
     * sends a message to a given connected slave
     * @param message
     * @param slave
     */
    void send_to(BaseMessage message, struct ConnectedSlave slave);

    /**
     * function to override, user can react to incoming messages
     * @param containers messages to handle
     */
    void handle_messages(std::vector<RawContainer> &containers);

    Server server;
private:
    std::string player_name;
    int PORT;
    int max_slaves_count;
    std::vector<struct ConnectedSlave> slaves;

    /**
     * checks if a player with given name is connected
     * @param name name of player to check
     * @return true if player with name is connected, false otherwise
     */
    bool player_connected(const std::string &name);
};

/**
 * slave class represents a slave
 * a slave can connect to one master and send messages to it
 * a slave listens for direct messages and broadcasts from the master
 */

class Slave {
public:
    /**
     * constructor
     * @param player_name name of the slave
     * @param port port to listen on for incoming messages
     */
    Slave(std::string player_name, int port);

    /**
     * runs one iteration of the slave
     * call in game loop
     * handles connecting and leaving and calls handle messages with new messages
     */
    void tick();

    /**
     * scans for running masters in local network
     * @param port port the masters are running on
     */
    std::vector<std::string> scan_for_masters(int port);

    /**
     * connects the slave to a master
     * @param ip ip of master
     * @param master_port port where master is reachable
     */
    void connect_to_master(std::string ip, int master_port);

    /**
     * sends a message to the master
     * @param message message to send to the master
     */
    void send(BaseMessage message);

    /**
     * function to override, user can react to incoming messages
     * @param containers messages to handle
     */
    virtual void handle_messages(std::vector<RawContainer> &containers);

    Server server;
private:
    int PORT;
    std::string player_name;

    bool connected;
    std::string MASTER_NAME;
    std::string MASTER_IP;
    int MASTER_PORT;

    std::vector<struct ConnectedSlave> other_slaves;
};

#endif //TEAMPRAKTIKUM_GAMEMANAGER_H
