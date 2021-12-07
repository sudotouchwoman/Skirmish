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
        Server::ConnectionServer cs([this](const boost::asio::ip::udp::endpoint &endpoint,
                                           const std::string &request) {
            return this->requestHandler(endpoint,
                                        request);
        });
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
    std::vector<std::shared_ptr<GameEntities::GameObject>> &go = _ge.getModifyGameObjects();
    go.push_back(std::make_shared<GameEntities::Player>());
    go.push_back(std::make_shared<GameEntities::Player>());
    go.push_back(std::make_shared<GameEntities::Player>());
    go.push_back(std::make_shared<GameEntities::Player>());
}

int onEvent(const ClientServer::MoveEvent &me) {
    // change position of player
}

int onEvent(const ClientServer::ShootEvent &se) {
    // make bullet
}

int onEvent(const ClientServer::InteractEvent &ie) {
    //do some logic
}

std::string GameServer::requestHandler(const boost::asio::ip::udp::endpoint &endpoint, const std::string &request) {
    // if player exist, than check events
    if (std::any_of(endpoint_id.begin(),
                    endpoint_id.end(),
                    [&endpoint](auto elem) {
                        if (elem.first == endpoint) return true;
                        return false;
                    }))

        switch (request[0]) {
            case ClientServer::Type::tCheck :break;
            case ClientServer::Type::tWalk : {
                // validate string
                const ClientServer::MoveEvent
                    *event = reinterpret_cast<const ClientServer::MoveEvent *>(request.data());
                onEvent(*event);
                break;
            }
            case ClientServer::Type::tShoot: {
                // validate string
                const ClientServer::ShootEvent
                    *event = reinterpret_cast<const ClientServer::ShootEvent *>(request.data());
                onEvent(*event);
                break;
            }
            default:return std::string();
        }
        // if not and event register - register player ( else ignore request)
    else if (request[0] == ClientServer::Type::tRegister) {
        GameEntities::Player pl;
//        pl.setModel()
        //создание объекта игрока, занесение его в базу отношений между айпи адресами
        // и прикрепление его к игровому пространству.
    }
    return _ge.getSnapshot();
}
