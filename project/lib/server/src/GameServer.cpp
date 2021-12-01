#include "GameServer.h"

#include <thread>
#include <iostream>

using Server::GameServer;

GameServer::GameServer() {
};

void GameServer::run() {
    initializeGE();
    try {
        Server::GameLoop gl(&_ge);
        Server::ConnectionServer cs([this](std::string str) { return this->requestHandler(std::move(str)); });
        std::thread game_loop_thread(&Server::GameLoop::run, &gl);
        std::thread server_thread(&Server::ConnectionServer::startReceive, &cs);
        game_loop_thread.join();
        server_thread.join();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void GameServer::initializeGE() {
    /// initialising
}

std::string GameServer::requestHandler(std::string){
    return "aobao";
}
