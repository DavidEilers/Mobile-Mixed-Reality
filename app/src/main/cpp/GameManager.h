//
// Created by Oskar on 22.02.2021.
//

#ifndef TEAMPRAKTIKUM_GAMEMANAGER_H
#define TEAMPRAKTIKUM_GAMEMANAGER_H

#include <vector>

#include "Network.h"

/**
 * struct to hold information about a connected slave
 * contains: name (string), ip (string) and port (int) of the slave
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
    void broadcast(struct Message message);

    /**
     * sends a message to a given connected slave
     * @param message
     * @param slave
     */
    void send_to(struct Message message, struct ConnectedSlave slave);

    Server server;
private:
    std::string player_name;
    int PORT;
    int max_slaves_count;
    std::vector<struct ConnectedSlave> slaves;
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
     * @param port port of master
     * @return returns an error code: 0 -> success, 1 -> connection error, 2 -> player name already in use, 3 -> game is full
     */
    int connect_to_master(std::string ip, int port);

    /**
     * sends a message to the master
     * @param message message to send to the master
     */
    void send(struct Message message);

    Server server;
private:
    int PORT;
    std::string player_name;

    bool connected;
    std::string master_ip;
    int master_port;
};

#endif //TEAMPRAKTIKUM_GAMEMANAGER_H
