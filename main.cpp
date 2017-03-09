#include "router.hpp"
#include "Settings.hpp"
#include <boost/lexical_cast.hpp>

#include <cstdint>

int main(int argc, char *argv[])
{
  try
  {
		// Read the settings from configuration file
		std::unique_ptr<Settings> appSettings(new Settings("settings.ini"));

		// Get the comms interface type
		std::string remote_ip = appSettings->getValue("Remote_IP");
		uint32_t remote_udp_port = 13;//boost::lexical_cast<uint32_t>(appSettings->getValue("Remote_IP"));
		std::string com_port = appSettings->getValue("COM");
		uint32_t baud_rate = 115200;//boost::lexical_cast<uint32_t>(appSettings->getValue("BAUD_RATE"));

    boost::asio::io_service io_service;
    SerialUdpRouter server(io_service, remote_ip, remote_udp_port, com_port, baud_rate);

    server.start_serial_receive();
    server.start_udp_receive();

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
