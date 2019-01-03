#include "pch.h"
#include "UDP_Server.h"

int main_udp_server()
{
	try
	{
		/*asio::io_context io_context;
		udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));
		udp::endpoint remote_endpoint;
		asio::error_code ignore_err;
		std::array<char, 1> recv_buf;

		while (true)
		{
			std::cout << "local endpoint : " << socket.local_endpoint().address() << std::endl;
			std::cout << "remote endpoint : " << remote_endpoint.address() << std::endl;
			socket.receive_from(asio::buffer(recv_buf), remote_endpoint);
			socket.send_to(asio::buffer(make_daytime_string()), remote_endpoint, 0, ignore_err);
			std::cout << "answer " << remote_endpoint.address() << " succeed." << std::endl;
			std::cout << std::string(30, '*') << std::endl;
		}*/

		asio::io_context io_context;
		udp_server server(io_context);
		io_context.run();
	}

	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}