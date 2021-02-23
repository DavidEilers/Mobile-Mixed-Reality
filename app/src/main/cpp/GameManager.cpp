//
// Created by Oskar on 22.02.2021.
//

#include <vector>

#include "GameManager.h"

//--------------Master-------------------

Master::Master(std::string player_name, int port, int max_slaves_count) : server(port) {
    this->player_name = player_name;
    this->PORT = port;
    this->max_slaves_count = max_slaves_count;
}

void Master::broadcast(struct Message message) {
    for (int i = 0; i < slaves.size(); i++) {
        send_to(message, slaves[i]);
    }
}

void Master::send_to(struct Message message, struct ConnectedSlave slave) {
    server.send_message(message, slave.ip, slave.port);
}

std::vector<struct ConnectedSlave> Master::get_connected_slaves() {
    std::vector<struct ConnectedSlave> copy(slaves);

    return copy;
}

//--------------Slave-------------------

Slave::Slave(std::string player_name, int port) : server(port) {
    this->player_name = player_name;
    this->PORT = port;

    connected = false;
}

int Slave::connect_to_master(std::string ip, int port) {

    //TODO

    master_ip = ip;
    master_port = port;

    connected = true;

    return 0;
}

void Slave::send(struct Message message) {
    if (connected) {
        server.send_message(message, master_ip, master_port);
    }
}