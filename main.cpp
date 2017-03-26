#include "router.hpp"
#include "Settings.hpp"
#include <boost/lexical_cast.hpp>
#include <iomanip>

///
/// \brief init : Initialize the parameters for connection pair
/// \param param : Parameters include local/remote IP address and
///                UDP Port, COM port and baud rate to attach to
///                the local & remote IP address.
/// \param index : Pair number from the settings tile
///
/// Method to initialize the connection parameters for Serial-Ethernet
/// conversion.
///
void init(serial_udp_router::connection_params &param, std::string index)
{

    //! Read the settings from configuration file
    std::unique_ptr<Settings> appSettings(new Settings("settings.ini"));

    std::string remote_ip = "Remote_IP_" + index;
    std::string remote_udp_port = "Remote_UDP_Port_" + index;
    std::string local_ip = "Local_IP_" + index;
    std::string local_udp_port = "Local_UDP_Port_" + index;
    std::string com_name = "COM_" + index;
    std::string baud_rate = "BAUD_RATE_" + index;

    //! Get remote IP address and UDP port from settings file
    param.remote_ip = appSettings->getValue(remote_ip);
    param.remote_udp_port = boost::lexical_cast<uint32_t>(appSettings->getValue(remote_udp_port));

    //! Get local IP address and UDP port from settings file
    param.local_ip = appSettings->getValue(local_ip);
    param.local_udp_port = boost::lexical_cast<uint32_t>(appSettings->getValue(local_udp_port));

    //! Get COM Port and baud rate from settings file
    param.com_port_name = appSettings->getValue(com_name);
    param.baud_rate = boost::lexical_cast<uint32_t>(appSettings->getValue(baud_rate));

    uint16_t width{45};
    std::cout << std::setw(width) << std::setfill('=') << "" << std::endl;
    std::cout << "PAIR - " << index << " Settings..." << std::endl;
    std::cout << "\tRemote IP Address:Port => " << param.remote_ip << ":" <<  param.remote_udp_port << std::endl;
    std::cout << "\tLocal IP Address:Port => " << param.local_ip << ":" <<  param.local_udp_port << std::endl;
    std::cout << "\tSerial COM Port => COM" << param.com_port_name << ", Baud Rate = " << param.baud_rate << std::endl;
    std::cout << std::setw(width) << "" << std::setfill(' ') << std::endl;
}

///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// Main method to create serial server objects.
int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service io_service;

        serial_udp_router::connection_params pair_1;
        serial_udp_router::connection_params pair_2;

        init(pair_1, "1");
        init(pair_2, "2");

        // Create the server object for pair 1
        serial_udp_router server(io_service, pair_1);

        // Start receiving serial data
        server.start_serial_receive();

        // Start receiving udp data
        server.start_udp_receive();

        // Create the server object for pair 2
        serial_udp_router server2(io_service, pair_2);
        server2.start_serial_receive();
        server2.start_udp_receive();

        // Wait for all async calls to be over
        // Because of the design of server object, this function will never return
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
