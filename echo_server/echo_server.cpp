#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

boost::asio::io_service service;

size_t handler_complete(char* buffer, const boost::system::error_code& err, size_t bytes) {
    bool found = std::find(buffer, buffer + bytes, '\n') < buffer + bytes;
    if (err || found) {
        return 0;
    }
    else
    {
        return 1;
    }
}

void connection() {
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 8090));
    char buff[1024];
    while (true)
    {
        boost::asio::ip::tcp::socket sock(service);
        acceptor.accept(sock);
        //        int bytes = read(sock, boost::asio::buffer(buff), boost::bind(handler_complete, buff, _1, _2));
        int bytes = read(sock, boost::asio::buffer(buff), boost::bind(handler_complete, buff, _1, _2));
        std::string reply(buff, bytes);
        std::cout << "message received: " << reply << std::endl;
        sock.write_some(boost::asio::buffer(reply));
        sock.close();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    connection();
}