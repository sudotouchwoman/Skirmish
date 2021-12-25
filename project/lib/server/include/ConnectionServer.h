#pragma once

#include "GameSettings.h"

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

namespace Server {
    class ConnectionServer {
        using OnMessageRecieveCallback = std::function <std::string(const boost::asio::ip::udp::endpoint &, const char *request)>;
    public:
        ConnectionServer(size_t port = std::stol(port_server));
        void startReceive();
        void stopReceive() {io_context_.stop(); };
        void MessageRecieveCallbackSetter(OnMessageRecieveCallback handle_message_);
    private:
        void asyncRecieve();
        void handleReceive(const boost::system::error_code &error,
                           std::size_t);

        void handleSend(const boost::system::error_code &);

        boost::asio::io_context io_context_;
        OnMessageRecieveCallback handle_message;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint remote_endpoint_;
        boost::array<char, max_transfer_event_bytes> recv_buffer_;
    };
}