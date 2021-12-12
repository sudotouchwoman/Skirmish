#include "GameServer.h"

#include <thread>
#include <iostream>
#include <utility>

using Server::GameServer;

GameServer::GameServer() {
    GameEntities::GameObject::resetID();
};

void GameServer::run() {
    try {
        Server::GameLoop gl(&_ge);
        Server::ConnectionServer cs([this](const boost::asio::ip::udp::endpoint &endpoint,
                                           const std::string &request) {
            return this->requestHandler(endpoint,
                                        request);
        });
        std::thread game_loop_thread(&Server::GameLoop::run, &gl);
        cs.startReceive();
        game_loop_thread.join();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::string GameServer::requestHandler(const boost::asio::ip::udp::endpoint &endpoint, const std::string &request) {
    _ge.getAccess();
    size_t player_id = 0;
    // if player exist, than check events
    if (std::any_of(endpoint_id.cbegin(),
                    endpoint_id.cend(),
                    [&endpoint, &player_id](auto & elem) {
                        if (get<0>(elem) == endpoint) {
                            player_id = get<1>(elem);
                            return true;
                        }
                        return false;
                    }))

        switch (request[0]) {
            case ClientServer::Type::tCheck :break;
            case ClientServer::Type::tWalk : {
                // validate string
                const ClientServer::MoveEvent
                    *event = reinterpret_cast<const ClientServer::MoveEvent *>(request.data());
                _ge.onEvent(player_id, *event);
                break;
            }
            case ClientServer::Type::tShoot: {
                // validate string
                const ClientServer::ShootEvent
                    *event = reinterpret_cast<const ClientServer::ShootEvent *>(request.data());
                _ge.onEvent(player_id, *event);
                break;
            }
            default: break;
        }

        // if not and event register - register player ( else send snapshot)
    else if (request[0] == ClientServer::Type::tRegister) {
        GameEntities::Player pl;
        size_t id = pl.getID();
        endpoint_id.emplace_back(endpoint, pl.getID());
        _ge.addPlayer(pl);
        return std::to_string(id);
    }

    _ge.finishAccess();

    return _ge.getSnapshot();
}
