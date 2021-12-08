#pragma once

#include "GameLoop.h"
#include "ConnectionServer.h"
#include "GlobalEnvironment.h"
#include "PlayerEvent.h"


#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <tuple>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace Server {

    class GameServer{
    public:
        GameServer();
        void run();
    private:
        std::string requestHandler(const boost::asio::ip::udp::endpoint &endpoint, const std::string &request);

        /// соответствие игровой сущности игрока и адреса с которого поступил реквест.
        std::vector<std::tuple<boost::asio::ip::udp::endpoint, int>> endpoint_id;
        GameEntities::GlobalEnvironment _ge;
    };

} // namespace Server