//! serial_udp_router class to route serial data to udp port and udp data to serial port

#include <iostream>
#include <string>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class serial_udp_router
{
    enum { MaxBufSize = 256 };

    //! Udp related private data
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    udp::endpoint local_endpoint;
    boost::array<char, MaxBufSize> recv_buffer_;

    //! Serial port related private data
    boost::asio::serial_port ser_port;
    std::string com_port_name;
    uint32_t baud_rate;
    boost::array<char, MaxBufSize> serial_buffer;

    void init();

public:

    //! Structure for storing the connection parameters
    struct connection_params
    {
        std::string remote_ip;
        uint32_t remote_udp_port;
        std::string local_ip;
        uint32_t local_udp_port;
        std::string com_port_name;
        uint32_t baud_rate;
    };

    //! Constructor function
    serial_udp_router(boost::asio::io_service& io_service, const connection_params&  param);

    void start_udp_receive();

    void handle_udp_receive(const boost::system::error_code& error,
                            std::size_t bytes_transferred);

    void udp_wirte_handler(
            const boost::system::error_code& error,
            std::size_t bytes_transferred);

    void start_serial_receive();

    void handle_serial_receive(const  boost::system::error_code& error,
                               std::size_t bytes_transferred);

    void serial_write_handler(const boost::system::error_code&, std::size_t);
};
