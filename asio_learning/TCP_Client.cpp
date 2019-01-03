#include "pch.h"

using asio::ip::tcp;

int main_tcp_client()
{
	try
	{
		asio::io_context io_context;
		tcp::resolver resolver(io_context);
		///可用的NTP服务器地址https://blog.csdn.net/weixin_37569048/article/details/80734804
		tcp::resolver::results_type endpoints = resolver.resolve("localhost", "daytime");
		tcp::socket socket(io_context);

		asio::connect(socket, endpoints);
		while(true)
		{
			std::array<char, 128> buf;
			asio::error_code error;

			size_t len = socket.read_some(asio::buffer(buf), error);
			if (error == asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw asio::system_error(error); // Some other error.

			std::cout.write(buf.data(), len);
		}
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}