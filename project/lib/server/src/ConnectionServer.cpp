#include "ConnectionServer.h"
#include "GameSettings.h"

#include <boost/thread.hpp>

using namespace boost::asio::ip;

namespace Server {
    ConnectionServer::ConnectionServer(size_t port)
        : io_context_(),
          socket_(io_context_, udp::endpoint(udp::v4(), port)) {
    }

    void ConnectionServer::waitForGameEnd() {
        std::mutex mut;
        std::unique_lock<std::mutex> lock(mut);
        while(!game_end)
        {
            cond.wait(lock, [this]{return game_end;});
        }
        stopReceive();
    }

    void ConnectionServer::sendMessage(const std::string &message, const boost::asio::ip::udp::endpoint &endpoint){
        socket_.send_to(boost::asio::buffer(message, message.length()), endpoint);
    }

    void ConnectionServer::MessageRecieveCallbackSetter(OnMessageRecieveCallback handle_message_){
        handle_message = handle_message_;
    }

    void ConnectionServer::startReceive() {
        std::thread ge(&ConnectionServer::waitForGameEnd, this);
        asyncRecieve();
        io_context_.run();
    }

    void ConnectionServer::asyncRecieve() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_, max_transfer_event_bytes), remote_endpoint_,
            [this](const boost::system::error_code& error, std::size_t bytes_transferred){this->handleReceive(error, bytes_transferred);});
    }

    void ConnectionServer::handleReceive(const boost::system::error_code &error,
                                         std::size_t) {
        if (!error || error == boost::asio::error::message_size) {
            socket_.async_send_to(boost::asio::buffer(handle_message(remote_endpoint_, recv_buffer_.data())),
                                  remote_endpoint_,
                                  [this](const boost::system::error_code& error, std::size_t bytes_transferred)
                                  {this->handleSend(error);});

            asyncRecieve();
        }
    }

    void ConnectionServer::handleSend(const boost::system::error_code &) {
    }
} // namespace Server
