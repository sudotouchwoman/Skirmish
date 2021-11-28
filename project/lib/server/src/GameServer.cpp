#include "GameServer.h"

#include <thread>
#include <boost/asio.hpp>
#include <iostream>

using Server::GameServer;

GameServer::GameServer() {
};

void GameServer::run() {
    initializeGE();
    try {
//        Server::GameLoop gl(&ge);
        Server::ConnectionServer cs([this](std::string) { return this->requestHandler("1"); });
//        std::thread game_loop_thread(&Server::GameLoop::run, &gl);
        std::thread server_thread(&Server::ConnectionServer::startReceive, &cs);
//        game_loop_thread.join();
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
