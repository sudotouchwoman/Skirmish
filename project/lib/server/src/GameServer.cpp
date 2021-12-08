#include "GameServer.h"

#include <thread>
#include <iostream>

using Server::GameServer;

GameServer::GameServer() {
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
    _ge.getAccess();
    // if player exist, than check events
    if (std::any_of(endpoint_id.begin(),
                    endpoint_id.end(),
                    [&endpoint](auto elem) {
                        if (get<0>(elem) == endpoint) return true;
                        return false;
                    }))

        switch (request[0] - 'a') {
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
            default: break;
        }
        // if not and event register - register player ( else ignore request)
    else if (request[0] - 'a' == ClientServer::Type::tRegister) {
        GameEntities::Player pl;
        auto model = std::make_unique<physical::PhysicalObject>();
        auto default_player_geometry = physical::IShapeUPtr(new core::Circle());
        model->setGeometry(std::move(default_player_geometry));
        pl.setModel(std::move(model));

        endpoint_id.emplace_back(endpoint, pl.getID());
        _ge.addPlayer(pl);
    }

    _ge.finishAccess();

    return _ge.getSnapshot();
}
