//
// Created by david on 10.04.21.
//
#include "arServer.h"

ArServer::ArServer() {
    arSession= nullptr;
    arFrame= nullptr;
    installRequested=false;
    imageID=-1;
    checkInstall();
}

void ArServer::checkInstall() {

}
ArServer::~ArServer() {}
void ArServer::onDrawFrame() {

}

