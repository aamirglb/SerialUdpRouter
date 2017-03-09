#include "router.hpp"

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service io_service;
        // Create the server object
        serial_udp_router server(io_service);

        // Start receiving serial data
        server.start_serial_receive();

        // Start receiving udp data
        server.start_udp_receive();

        // Wait for all async calls to be over
        // Because of the design of server object, this function will never return
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
