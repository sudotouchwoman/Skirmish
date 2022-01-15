#include "ConnectionClient.h"
#include "GameSettings.h"

using namespace boost::asio::ip;

namespace Client {

    ConnectionClient::ConnectionClient(const std::string &host, const std::string &port, const size_t port_client)
        : io_context_(),
          socket_(io_context_, udp::endpoint(udp::v4(), port_client)) {
        udp::resolver resolver(io_context_);
        remote_endpoint_ =
            resolver.resolve(udp::v4(), host, port);
    }

    ConnectionClient::ConnectionClient(size_t
                                       port_client) :
        io_context_(),
        socket_(io_context_, udp::endpoint(udp::v4(), port_client)) {
        udp::resolver resolver(io_context_);
        remote_endpoint_ =
            resolver.resolve(udp::v4(), host_server, port_server);
    }

    void ConnectionClient::setSnapshotRecieveCallback(const std::function<void(std::string)> &handler) {
        handle_snapshot = handler;
    }

    size_t ConnectionClient::registerPlayer() {
        char request[1];
        request[0] = ClientServer::Type::REGISTER;

        int request_length = 1;
        socket_.send_to(boost::asio::buffer(request, request_length), *remote_endpoint_.begin());
        char reply[udp_size];
        udp::endpoint sender_endpoint;
        size_t reply_length = socket_.receive_from(
            boost::asio::buffer(reply, udp_size), sender_endpoint);

        return std::stol(reply);
    }

    void ConnectionClient::send(const std::string &str) {
        socket_.send_to(boost::asio::buffer(str, str.length()), *remote_endpoint_.begin());
    }

    void ConnectionClient::getReply() {
        char reply[udp_size];
        udp::endpoint sender_endpoint;
        size_t reply_length = socket_.receive_from(
            boost::asio::buffer(reply, udp_size), sender_endpoint);

        // possible error handling
        if (reply_length == 0)
            return;

        handle_snapshot(std::string(reply, reply_length));
    }

    void ConnectionClient::sendEvent(ClientServer::MoveEvent &ev) {
        char type = ClientServer::Type::WALK;
        char *data = reinterpret_cast<char *>(&ev);
        std::string request = (std::string() + type).append(data, sizeof(ev));
        send(request);
        getReply();
    }

    void ConnectionClient::sendEvent() {
        char request[1];
        request[0] = ClientServer::Type::CHECK;

        send(std::string(request, 1));
        getReply();
    }

    void ConnectionClient::sendEvent(ClientServer::RotateEvent &ev) {
        char type = ClientServer::Type::ROTATE;
        char *data = reinterpret_cast<char *>(&ev);
        std::string request = (std::string() + type).append(data, sizeof(ev));
        send(request);
        getReply();
    }

    void ConnectionClient::sendEvent(ClientServer::ShootEvent &ev) {
        char type = ClientServer::Type::SHOOT;
        char *data = reinterpret_cast<char *>(&ev);
        std::string request = (std::string() + type).append(data, sizeof(ev));
        send(request);
        getReply();
    }

    void ConnectionClient::sendEvent(ClientServer::InteractEvent &ev) {
        char *data = reinterpret_cast<char *>(&ev);
        send(std::string(data, sizeof(ev)));
        getReply();
    }
} // namespace client

