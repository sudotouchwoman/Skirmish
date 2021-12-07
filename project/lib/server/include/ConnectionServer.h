#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

using namespace boost::asio::ip;

namespace Server {
    class ConnectionServer {
    public:
        ConnectionServer(std::function<std::string(const boost::asio::ip::udp::endpoint &, std::string)> handle_message_);
        void startReceive();
    private:
        void handleReceive(const boost::system::error_code &error,
                           std::size_t /*bytes_transferred*/);

        void handleSend(boost::shared_ptr<std::string> /*message*/,
                        const boost::system::error_code & /*error*/,
                        std::size_t /*bytes_transferred*/);

        boost::asio::io_context io_context_;
        std::function<std::string(const boost::asio::ip::udp::endpoint &, std::string)> handle_message;
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::array<char, 1> recv_buffer_;
    };
}