/*
 * router.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: aamir.ali
 */

#include "router.hpp"
#include "Settings.hpp"
#include <boost/lexical_cast.hpp>

#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace boost;
using boost::asio::ip::udp;

//!
//! \brief serial_udp_router::init
//! Initialization function to initialize class private data from
//! configuration file
//!
void serial_udp_router::init()
{
    //! Read the settings from configuration file
    std::unique_ptr<Settings> appSettings(new Settings("settings.ini"));

    //! Get remote IP address and UDP port from settings file
    std::string remote_ip = appSettings->getValue("Remote_IP");
    uint32_t remote_udp_port = boost::lexical_cast<uint32_t>(appSettings->getValue("Remote_UDP_Port"));

    //! Get local IP address and UDP port from settings file
    std::string local_ip = appSettings->getValue("Local_IP");
    uint32_t local_udp_port = boost::lexical_cast<uint32_t>(appSettings->getValue("Local_UDP_Port"));

    //! Get COM Port and baud rate from settings file
    com_port_name = appSettings->getValue("COM");
    baud_rate = boost::lexical_cast<uint32_t>(appSettings->getValue("BAUD_RATE"));

    //! Set remote endpoint value
    remote_endpoint_.address(boost::asio::ip::address::from_string(remote_ip));
    remote_endpoint_.port(remote_udp_port);

    //! Set local endpoint value
    local_endpoint.address(boost::asio::ip::address::from_string(local_ip));
    local_endpoint.port(local_udp_port);

    uint16_t width{45};
    std::cout << std::setw(width) << std::setfill('=') << "" << std::endl;
    std::cout << "Remote IP Address:Port => " << remote_ip << ":" <<  remote_udp_port << std::endl;
    std::cout << "Local IP Address:Port => " << local_ip << ":" <<  local_udp_port << std::endl;
    std::cout << "Serial COM Port => COM" << com_port_name << ", Baud Rate = " << baud_rate << std::endl;
    std::cout << std::setw(width) << "=" << std::setfill(' ') << std::endl;
}

//!
//! \brief serial_udp_router::serial_udp_router
//! \param io_service
//! Class constructor responsible for opening the local socket and binding
//! local ip address and udp port
//!
serial_udp_router::serial_udp_router(boost::asio::io_service& io_service) :
    socket_(io_service), ser_port(io_service)
{
    // Initialise class variables
    init();

    socket_.open(local_endpoint.protocol());
    //socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(local_endpoint);

    std::string serial_port("COM");
    serial_port += com_port_name;

    ser_port.open(serial_port);
    ser_port.set_option(asio::serial_port_base::baud_rate(baud_rate));
}

//!
//! \brief serial_udp_router::start_udp_receive
//! Method to start listening for UDP data on local port asynchronously.
//!
void serial_udp_router::start_udp_receive()
{
    socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                boost::bind(&serial_udp_router::handle_udp_receive, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

//!
//! \brief serial_udp_router::handle_udp_receive
//! \param error
//! \param bytes_transferred
//! Method which gets called after receiving the data on UDP socket.
//! This method is responsible for forwarding the received UDP data on
//! serial interface.
//!
void serial_udp_router::handle_udp_receive(const boost::system::error_code& error,
                                           std::size_t bytes_transferred)
{
    //std::cout << recv_buffer_ << "\n";
    ser_port.async_write_some(boost::asio::buffer(recv_buffer_, bytes_transferred),
                              boost::bind(&serial_udp_router::serial_write_handler, this,
                                          boost::asio::placeholders::error(),
                                          boost::asio::placeholders::bytes_transferred()));

    //! Again start listening for UDP data to keep io_service.run() busy
    start_udp_receive();
}

//!
//! \brief serial_udp_router::serial_write_handler
//! This method is only used as a place holder handler hence it is not used.
void serial_udp_router::serial_write_handler(
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/)
{
    //std::cout << bytes_transferred << " written successfully\n";
}

//!
//! \brief serial_udp_router::udp_wirte_handler
//! This method is only used as a place holder handler hence it is not used.
//!
void serial_udp_router::udp_wirte_handler(
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/)
{
}

//!
//! \brief serial_udp_router::start_serial_receive
//! Method to start listening for UDP data on local port asynchronously.
//!
void serial_udp_router::start_serial_receive()
{
    //std::cout << "Starting serial server...\n";
    ser_port.async_read_some(boost::asio::buffer(serial_buffer),
                             boost::bind(&serial_udp_router::handle_serial_receive,
                                         this,
                                         boost::asio::placeholders::error(),
                                         boost::asio::placeholders::bytes_transferred()));
}

//!
//! \brief serial_udp_router::handle_serial_receive
//! \param error
//! \param bytes_transferred
//! Method which gets called after receiving the data on serial interface.
//! This method is responsible for forwarding the received serial data on
//! UDP interface.
//!
void serial_udp_router::handle_serial_receive(const  boost::system::error_code& error,
                                              std::size_t bytes_transferred)
{

    socket_.async_send_to(boost::asio::buffer(serial_buffer, bytes_transferred), remote_endpoint_,
                          boost::bind(&serial_udp_router::udp_wirte_handler, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));

    //! Again start listening for serial data to keep io_service.run() busy
    start_serial_receive();
}
