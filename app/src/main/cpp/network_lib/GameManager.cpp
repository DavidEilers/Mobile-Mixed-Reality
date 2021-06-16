//
// Created by Oskar on 22.02.2021.
//

#include <vector>
#include <android/log.h>
#include "GameManager.h"

//--------------Master-------------------

Master::Master(std::string player_name, int port, int max_slaves_count) : server(port) {
    this->player_name = player_name;
    this->PORT = port;
    this->max_slaves_count = max_slaves_count;
}

void Master::broadcast(BaseMessage *message) {
    for (int i = 0; i < slaves.size(); i++) {
        send_to(message, slaves[i]);
    }
}

void Master::send_to(BaseMessage *message, struct ConnectedSlave slave) {
    server.send_message(message, slave.ip, slave.port);
}

std::vector<struct ConnectedSlave> Master::get_connected_slaves() {
    std::vector<struct ConnectedSlave> copy(slaves);

    return copy;
}

void Master::handle_messages(std::vector<RawContainer> &containers) {
    //override by user
}

bool Master::player_connected(const std::string &name) {
    //master is always connected
    if (name == player_name) return true;
    //check if a player with this name is connected
    for (const auto &player:slaves) {
        if (player.name == name)
            return true;
    }
    return false;
}

void Master::tick() {
    auto containers = server.get_messages();
    for (const auto &container:containers) {
        ConnectMessage cm;
        if (cm.from_bytes(container.buffer)) {
            //add player if name unused
            struct ConnectedSlave new_slave;

            new_slave.name = cm.player_name;
            new_slave.ip = container.from_addr;
            new_slave.port = cm.port;

            if (acceptConnections) {
                if (!player_connected(cm.player_name)) {
                    //name free
                    if (slaves.size() < max_slaves_count) {
                        slaves.push_back(new_slave);
                        //reusing cm object but filling it with info about master
                        cm.player_name = player_name;
                        cm.port = PORT;
                        send_to(&cm, new_slave);
                    } else {
                        ErrorGameFullMessage em;
                        send_to(&em, new_slave);
                    }
                } else {
                    //name already in use
                    ErrorNameInUseMessage error;
                    send_to(&error, new_slave);
                }
            } else {
                //no new connections allowed
                ErrorGameFullMessage em;
                send_to(&em, new_slave);
            }
        }
        LeaveMessage lm;
        if (lm.from_bytes(container.buffer)) {
            if (player_connected(lm.player_name)) {
                for (int i = 0; i < slaves.size(); i++) {
                    if (slaves[i].name == lm.player_name) {
                        slaves.erase(slaves.begin() + i);
                        break;
                    }
                }
            }
        }
    }

    //override by user
    handle_messages(containers);

    //delete buffers from heap
    for (const auto &container:containers) {
        delete container.buffer;
    }
    containers.clear();
}

void Master::disconnectAllSlaves() {
    LeaveMessage lm;
    lm.player_name = player_name;
    broadcast(&lm);
    slaves.clear();
}

//--------------Slave-------------------

Slave::Slave(std::string player_name, int port) : server(port) {
    this->player_name = player_name;
    this->PORT = port;

    connected = false;
}

void Slave::connect_to_master(std::string ip, int master_port) {

    ConnectMessage cm;

    cm.player_name = player_name;
    MASTER_IP=ip;
    MASTER_PORT=master_port;
    cm.port = PORT; //own port, master can reach us here

    server.send_message(&cm, ip, master_port);
    server.start_listening();
}

void Slave::send(BaseMessage *message) {
    if (connected) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikumNetwork", "Sending Message");
        server.send_message(message, MASTER_IP, MASTER_PORT);
    }
}

void Slave::handle_messages(std::vector<RawContainer> &containers){
    //override by user
}

void Slave::tick() {
    auto containers = server.get_messages();
    for (const auto &container:containers) {

        __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikumNetwork","Got a networkPacket");
        //Master accepts connection
        ConnectMessage cm;
        if (cm.from_bytes(container.buffer)) {
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikumNetwork", "Got a networkPacket");
            MASTER_IP = container.from_addr;
            MASTER_PORT = cm.port;
            MASTER_NAME = cm.player_name;
            connected = true;
        }
        //Master disconnects this slave
        LeaveMessage lm;
        if (lm.from_bytes(container.buffer)) {
            connected = false;
        }

        ErrorGameFullMessage egm;
        if (egm.from_bytes(container.buffer)) {
            connected = false;
        }
    }

    //override by user
    handle_messages(containers);

    //delete buffers from heap
    for (const auto &container:containers) {
        delete container.buffer;
    }
    containers.clear();
}

void Slave::disconnect() {
    if (connected) {
        connected = false;
        LeaveMessage lm;
        lm.player_name = player_name;
        send(&lm);
    }
}