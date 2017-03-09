//! SerialUdpRouter class to route serial data to udp port and udp data to serial port


#include <iostream>
#include <string>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class SerialUdpRouter
{
	//! Udp related private data
	  udp::socket socket_;
	  udp::endpoint remote_endpoint_;

	  boost::array<char, 256> recv_buffer_;
	  std::string raw_ip_address;
	  unsigned short port_num;

	  //! Serial port related private data
	  boost::asio::serial_port ser_port;
	  boost::array<char, 256> serial_buffer;

public:

	SerialUdpRouter(boost::asio::io_service& io_service,
		std::string remote_ip,
		uint32_t remote_port,
		std::string com_port,
	uint32_t baud_rate);

	void start_udp_receive();

	void handle_udp_receive(const boost::system::error_code& error,
	std::size_t /*bytes_transferred*/);

	void udp_wirte_handler(
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/);

	void start_serial_receive();
	void handle_serial_receive(const  boost::system::error_code& error,
			   std::size_t bytes_transferred);

	void serial_write_handler(
	  const boost::system::error_code& error, // Result of operation.
	  std::size_t bytes_transferred           // Number of bytes written.
	);

};
