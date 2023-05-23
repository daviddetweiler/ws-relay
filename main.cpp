#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

using server_type = websocketpp::server<websocketpp::config::asio>;
using message_ptr = server_type::message_ptr;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;

class relay
{
public:
    void init()
    {
        server.init_asio();
        server.set_message_handler(
            websocketpp::lib::bind(&relay::on_message, this, ::_1, ::_2));

        server.set_open_handler(
            websocketpp::lib::bind(&relay::on_open, this, ::_1));

        server.listen(9002);
        server.start_accept();
        std::cerr << "Starting websocket server\n";
        server.run();
        std::cerr << "Stopping websocket server\n";
    }

private:
    server_type server{};

    void on_message() {
        std::cerr << "Message received\n";
    }

    void on_open() {
        std::cerr << "Connection opened\n";
    }
}

int
main()
{
}
