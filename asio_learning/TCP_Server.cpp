#include "pch.h"
#include "TCP_Server.h"

int main_tcp_server()
{
	try
	{
		/*asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
		tcp::socket socket(io_context);
		asio::error_code ignore_err;

		while (true)
		{
			tcp::socket socket(io_context);
			
			acceptor.accept(socket);
			std::cout << "local endpoint : " << socket.local_endpoint().address() << std::endl;
			std::cout << "remote endpoint : " << socket.remote_endpoint().address() << std::endl;
			asio::write(socket, asio::buffer(make_daytime_string()), ignore_err);
			std::cout << "answer " << socket.remote_endpoint().address() << " succeed." << std::endl;
			std::cout << std::string(30, '*') << std::endl;
		}*/

		asio::io_context io_context;
		tcp_server server(io_context);
		io_context.run();
	}
	
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}