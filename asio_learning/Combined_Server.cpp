#include "pch.h"
#include "TCP_Server.h"
#include "UDP_Server.h"

int main()
{
	try
	{
		asio::io_context io_context;
		tcp_server server1(io_context);
		udp_server server2(io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}