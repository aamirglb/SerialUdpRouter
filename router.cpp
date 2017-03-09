/*
 * router.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: aamir.ali
 */

#include "router.hpp"

using namespace boost;
using boost::asio::ip::udp;

SerialUdpRouter::SerialUdpRouter(boost::asio::io_service& io_service,
	std::string remote_ip,
	uint32_t remote_port,
	std::string com_port,
	uint32_t baud_rate) :
	socket_(io_service), ser_port(io_service)
{
	raw_ip_address = remote_ip;
	port_num = 13;

    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), raw_ip_address, "daytime");
    remote_endpoint_ = *resolver.resolve(query);

	boost::asio::ip::udp::endpoint local_endpoint(
        boost::asio::ip::address::from_string("172.22.6.55"), 8081);
    socket_.open(local_endpoint.protocol());
    //socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(local_endpoint);

	std::string serial_port("COM");
	serial_port += com_port;

    ser_port.open(serial_port);
	ser_port.set_option(asio::serial_port_base::baud_rate(baud_rate));
}


void SerialUdpRouter::start_udp_receive()
{
	std::cout << "Starting udp server...\n";
  socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), remote_endpoint_,
      boost::bind(&SerialUdpRouter::handle_udp_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void SerialUdpRouter::handle_udp_receive(const boost::system::error_code& error,
    std::size_t bytes_transferred)
{
	//std::cout << recv_buffer_ << "\n";
	ser_port.async_write_some(boost::asio::buffer(recv_buffer_, bytes_transferred),
	       boost::bind(&SerialUdpRouter::serial_write_handler, this,
	       boost::asio::placeholders::error(),
           boost::asio::placeholders::bytes_transferred()));

    start_udp_receive();
}

void SerialUdpRouter::serial_write_handler(
  const boost::system::error_code& error, // Result of operation.
  std::size_t bytes_transferred           // Number of bytes written.
)
{
	//std::cout << bytes_transferred << " written successfully\n";
}

void SerialUdpRouter::udp_wirte_handler(
    const boost::system::error_code& /*error*/,
    std::size_t /*bytes_transferred*/)
{
}

void SerialUdpRouter::start_serial_receive()
{
	//std::cout << "Starting serial server...\n";
    ser_port.async_read_some(boost::asio::buffer(serial_buffer),
            boost::bind(&SerialUdpRouter::handle_serial_receive,
            		this,
                    boost::asio::placeholders::error(),
                    boost::asio::placeholders::bytes_transferred()));
}

void SerialUdpRouter::handle_serial_receive(const  boost::system::error_code& error,
                   std::size_t bytes_transferred)
{
	//std::cout << "Serial data received on COM2...\n";
    //boost::array<char, 1> send_buf  = { 0 };
    //socket_.send_to(boost::asio::buffer(send_buf), remote_endpoint_);

/*boost::asio::ip::udp::endpoint remote_endpoint_(
    boost::asio::ip::address::from_string("172.22.6.55"), 13);
socket.send_to(boost::asio::buffer(data, size), destination);*/
	// Write to udp port
    socket_.async_send_to(boost::asio::buffer(serial_buffer, bytes_transferred), remote_endpoint_,
        boost::bind(&SerialUdpRouter::udp_wirte_handler, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

	start_serial_receive();
}
