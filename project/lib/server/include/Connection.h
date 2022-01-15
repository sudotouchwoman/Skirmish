#pragma once

#include "GameLoop.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>

namespace Server {

    class Connection
        : public boost::enable_shared_from_this<Connection>,
          private boost::noncopyable {
    public:
        /// Construct a connection with the given io_context.
        explicit Connection(boost::asio::io_context &io_context);

        /// Get the socket associated with the connection.
        boost::asio::ip::udp::socket &socket();

        /// Start the first asynchronous operation for the connection.
        void start();

    private:
        /// Handle completion of a read operation.
        void handle_read(const boost::system::error_code &e,
                         std::size_t bytes_transferred);

        /// Handle completion of a write operation.
        void handle_write(const boost::system::error_code &e);

    private:
        /// Strand to ensure the connection's handlers are not called concurrently.
        boost::asio::strand<boost::asio::io_context::executor_type> strand_;

        /// Socket for the connection.
        boost::asio::ip::udp::socket socket_;

        /// Buffer for incoming data.
        boost::array<char, 8192> buffer_;

        /// Game loop object
        Server::Serializer &game_loop_;
    };
} // namespace server3