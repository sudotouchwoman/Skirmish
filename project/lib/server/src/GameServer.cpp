#include "GameServer.h"
#include "GameLoop.h"
#include "ConnectionServer.h"
#include "GameSettings.h"

#include <thread>
#include <iostream>

namespace Server {

    GameServer::GameServer() {
        GameEntities::GameObject::resetID();
    };

    void GameServer::run() {
        try {
            Server::GameLoop gl(&_ge);
            Server::ConnectionServer cs;
            cs.MessageRecieveCallbackSetter([this](const boost::asio::ip::udp::endpoint &endpoint,
                                               const char *request) {
                return this->requestHandler(endpoint,
                                            request);
            });

            // fill global environment with obstacles
            for (auto &obstacle: obstacle_config__1)
                _ge.addObstacle(obstacle[0], obstacle[1], obstacle[2], obstacle[3]);

            std::thread game_loop_thread(&Server::GameLoop::run, &gl);
            cs.startReceive();
            game_loop_thread.join();
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::string GameServer::requestHandler(const boost::asio::ip::udp::endpoint &endpoint, const char *&request) {
        size_t player_id = 0;

        // if player exist, than check events
        auto &endpoint_string = endpoint;//.address().to_string();
        auto endpoint_id_element = endpoint_id.find(endpoint_string);
        if (endpoint_id_element != endpoint_id.end()) {
            player_id = endpoint_id_element->second;

            // if player not dead, than check events.
            if (!_ge.getPlayerById(player_id).isDead())
            // check events
            switch (request[0]) {
                case ClientServer::Type::CHECK :break;
                case ClientServer::Type::WALK : {
                    // validate string
                    const ClientServer::MoveEvent
                        *event = reinterpret_cast<const ClientServer::MoveEvent *>(request + 1);
                    _ge.onEvent(player_id, *event);
                    break;
                }
                case ClientServer::Type::ROTATE : {
                    // validate string
                    const ClientServer::RotateEvent
                        *event = reinterpret_cast<const ClientServer::RotateEvent *>(request + 1);
                    _ge.onEvent(player_id, *event);
                    break;
                }
                case ClientServer::Type::SHOOT: {
                    // validate string
                    const ClientServer::ShootEvent
                        *event = reinterpret_cast<const ClientServer::ShootEvent *>(request + 1);
                    _ge.onEvent(player_id, *event);
                    break;
                }
                case ClientServer::Type::REGISTER:return std::to_string(-1);
                default: break;
            }
        }
            // if not and event register - register player ( else send snapshot)
        else if (request[0] == ClientServer::Type::REGISTER) {
            const ClientServer::RegisterEvent
                *event = reinterpret_cast<const ClientServer::RegisterEvent *>(request + 1);
            size_t id = _ge.addPlayer(*event);
            endpoint_id[endpoint_string] = id;

            return std::to_string(id);
        }
        // not return value because there is need to
        // do it in restrict access and not to copy
        // value twice
        std::string response;
        _ge.getSnapshot(response);

        return response;
    }
} // namespace Server
