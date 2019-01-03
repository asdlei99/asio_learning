#include "pch.h"

using asio::ip::udp;

int main_udp_client()
{
	try
	{
		asio::io_context io_context;
		udp::resolver resolver(io_context);
		///可用的NTP服务器地址https://blog.csdn.net/weixin_37569048/article/details/80734804
		udp::endpoint endpoint = *resolver.resolve(udp::v4(), "localhost", "daytime").begin();
		udp::socket socket(io_context);

		socket.open(udp::v4());
		socket.send_to(asio::buffer("What's the time now ?"), endpoint);

		udp::endpoint sender_endpoint;
		std::array<char, 128> recv_buf;
		size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

		std::cout.write(recv_buf.data(), len);
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}