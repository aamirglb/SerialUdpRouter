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
void init(const std::unique_ptr<Settings> &appSettings,
          serial_udp_router::connection_params &param,
          std::string index)
{
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
        //! Read the settings from configuration file
        std::unique_ptr<Settings> appSettings(new Settings("settings.ini"));
        const uint32_t no_of_connections = boost::lexical_cast<uint32_t>(appSettings->getValue("NO_OF_CONNECTIONS"));
        std::cout << "No of Connections: " << no_of_connections << std::endl;

        // Boost IO Service object for Async communication
        boost::asio::io_service io_service;

        // Create an array of pointers with required number of connections
        serial_udp_router *server[no_of_connections];

        // UDP-Serial setting for each connection
        serial_udp_router::connection_params conn_settings;

        // Create server objects
        for(uint32_t i = 0; i < no_of_connections; ++i)
        {
            init(appSettings, conn_settings, std::to_string(i+1));
            server[i] = new serial_udp_router(io_service, conn_settings);
        }

        // Start serial and udp service
        for(uint32_t i = 0; i < no_of_connections; ++i)
        {
            server[i]->start_serial_receive();
            server[i]->start_udp_receive();
        }

        // Wait for all async calls to be over
        // Because of the design of server object, this function will never return
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
