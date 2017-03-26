/*
 * router.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: aamir.ali
 */

#include "router.hpp"

#include <cstdint>
#include <iostream>

using namespace boost;
using boost::asio::ip::udp;

//!
//! \brief serial_udp_router::serial_udp_router
//! \param io_service
//! Class constructor responsible for opening the local socket and binding
//! local ip address and udp port
//!
serial_udp_router::serial_udp_router(boost::asio::io_service& io_service,
                                     const connection_params &param) :
    socket_(io_service), ser_port(io_service)
{

    //! Set remote endpoint value
    remote_endpoint_.address(boost::asio::ip::address::from_string(param.remote_ip));
    remote_endpoint_.port(param.remote_udp_port);

    //! Set local endpoint value
    local_endpoint.address(boost::asio::ip::address::from_string(param.local_ip));
    local_endpoint.port(param.local_udp_port);

    socket_.open(local_endpoint.protocol());
    //socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(local_endpoint);

    std::string serial_port("COM");
    serial_port += param.com_port_name;

    ser_port.open(serial_port);
    ser_port.set_option(asio::serial_port_base::baud_rate(param.baud_rate));
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

    //std::cout << "serial server: rx " << bytes_transferred << std::endl;
    //! Again start listening for serial data to keep io_service.run() busy
    start_serial_receive();
}
