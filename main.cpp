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
    void init(std::uint16_t port, relay *sink = nullptr)
    {
        this->sink = sink;

        server.init_asio();
        server.set_message_handler(
            websocketpp::lib::bind(&relay::on_message, this, ::_1, ::_2));

        server.set_open_handler(
            websocketpp::lib::bind(&relay::on_open, this, ::_1));

        server.listen(port);
        server.start_accept();
        std::cerr << "Starting websocket server\n";
        server.run();
        std::cerr << "Stopping websocket server\n";
    }

private:
    server_type server{};
    relay *sink{};
    server_type::connection_ptr connection{};

    void on_message(websocketpp::connection_hdl, message_ptr message)
    {
        std::cerr << "Message received\n";
        if (sink && sink->connection && message->get_opcode() == 1)
        {
            std::cerr << "Echoing packet\n";
            sink->connection->send(message);
        }
    }

    void on_open(websocketpp::connection_hdl c)
    {
        std::cerr << "Connection opened\n";
        connection = server.get_con_from_hdl(c);
    }
};

int main()
{
    relay relay_sink{};
    relay relay_source{};
    std::thread a{
        [&]
        {
            relay_sink.init(9002, &relay_source);
        }};

    std::thread b{
        [&]
        {
            relay_source.init(9003, &relay_sink);
        }};

    a.join();
    b.join();
}
