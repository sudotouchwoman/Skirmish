#include "ConnectionServer.h"
#include "GameSettings.h"

using namespace boost::asio::ip;

using Server::ConnectionServer;

ConnectionServer::ConnectionServer(std::function<std::string(const boost::asio::ip::udp::endpoint &,
                                                             const char *request)> handle_message_)
    : io_context_(),
      socket_(io_context_, udp::endpoint(udp::v4(), 5000)) {
    handle_message = handle_message_;
}

void ConnectionServer::startReceive() {
    asyncRecieve();
    io_context_.run();
}

void ConnectionServer::asyncRecieve() {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_, max_transfer_event_bytes), remote_endpoint_,
        boost::bind(&ConnectionServer::handleReceive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void ConnectionServer::handleReceive(const boost::system::error_code &error,
                                     std::size_t /*bytes_transferred*/) {
    if (!error || error == boost::asio::error::message_size) {
//        boost::shared_ptr<std::string> message(
//            new std::string(handle_message(remote_endpoint_, recv_buffer_.data())));
        socket_.async_send_to(boost::asio::buffer(handle_message(remote_endpoint_, recv_buffer_.data())), remote_endpoint_,
                              boost::bind(&ConnectionServer::handleSend, this, //message,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));

        asyncRecieve();
    }
}

void ConnectionServer::handleSend(//boost::shared_ptr<std::string> /*message*/,
                                  const boost::system::error_code & /*error*/,
                                  std::size_t /*bytes_transferred*/) {
}