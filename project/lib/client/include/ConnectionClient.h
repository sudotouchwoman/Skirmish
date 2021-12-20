#pragma once

#include "PlayerEvent.h"
#include "GameSettings.h"

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

using namespace boost::asio::ip;

namespace Client {
    class ConnectionClient {
    public:
        ConnectionClient(const std::string &host = host_server, const std::string &port = port_server, const size_t port_client = default_client_port);
        ConnectionClient(const size_t port_client);
        void setSnapshotRecieveCallback(const std::function<void(std::string)> &);

        void sendEvent(ClientServer::MoveEvent &ev);
        void sendEvent();
        void sendEvent(ClientServer::ShootEvent &ev);
        void sendEvent(ClientServer::RotateEvent &ev);
        void sendEvent(ClientServer::InteractEvent &ev);

        //register player and get his id
        size_t registerPlayer(const char *name, const int texture_id);

    private:
        void send(const std::string &str);
        void getReply();

        boost::asio::io_context io_context_;
        std::function<void(std::string)> handle_snapshot;
        udp::socket socket_;
        udp::resolver::results_type remote_endpoint_;
    };
}