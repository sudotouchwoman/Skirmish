#include "Server.h"

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>

namespace Server {

    server::server(const std::string &address, const std::string &port,
                   std::size_t thread_pool_size)
        : thread_pool_size_(thread_pool_size),
          signals_(io_context_),
          acceptor_(io_context_),
          new_connection_() {
        // Register to handle the signals that indicate when the server should exit.
        signals_.add(SIGINT);   // остановка процесса с терминала
        signals_.add(SIGTERM);  // сигнал от kill
        signals_.async_wait(boost::bind(&server::handle_stop, this));

        request_router.addHandler("/posts", HandlerGetPost);
        request_router.addHandler("/user", HandlerGetUser);

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }

    void server::run() {
        // Create a pool of threads to run all of the io_contexts.
        std::vector<boost::shared_ptr<std::thread> > threads;
        for (std::size_t i = 0; i < thread_pool_size_; ++i) {
            boost::shared_ptr<std::thread> thread(new std::thread(
                boost::bind(&boost::asio::io_context::run, &io_context_)));
            threads.push_back(thread);
        }

        // Wait for all threads in the pool to exit.
        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }

    void server::start_accept() {
        new_connection_.reset(new Connection(io_context_, request_router));
        acceptor_.async_accept(new_connection_->socket(),
                               boost::bind(&server::handle_accept, this,
                                           boost::asio::placeholders::error));
    }

    void server::handle_accept(const boost::system::error_code &e) {
        if (!e) {
            new_connection_->start();
        }

        start_accept();
    }

    void server::handle_stop() {
        io_context_.stop();
    }

} // namespace Server
