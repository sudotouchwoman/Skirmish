#pragma once

#include "GlobalEnvironment.h"

#include <boost/asio.hpp>

namespace Server {

    class GameServer{
    public:
        GameServer();
        void run();
    private:
        std::string requestHandler(const boost::asio::ip::udp::endpoint &endpoint, const char * &request);

        /// соответствие игровой сущности игрока и адреса с которого поступил реквест.
        std::unordered_map<boost::asio::ip::udp::endpoint, size_t> endpoint_id;
        GameEntities::GlobalEnvironment _ge;
    };

} // namespace Server