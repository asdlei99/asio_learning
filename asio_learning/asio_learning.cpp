// asio_learning.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

///1
asio::io_service io_service;
asio::io_context::strand io_strand(io_service);
///2
asio::ip::tcp::resolver resolver(io_service);
asio::ip::tcp::socket sock(io_service);
std::array<char, 265> buffer;
///3
asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 80);
asio::ip::tcp::acceptor acceptor(io_service, endpoint);
std::string data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";

///1
void handler(const asio::error_code &ec)
{
	std::cout << "threadid: " << std::this_thread::get_id() << std::endl;
}

void run()
{
	io_service.run();
}

///2
void read_handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout << std::string(buffer.data(), bytes_transferred) << std::endl;
		sock.async_read_some(asio::buffer(buffer), read_handler);
	}
}

void connect_handler(const asio::error_code &ec)
{
	if (!ec)
	{
		asio::write(sock, asio::buffer("GET / HTTP 1.1\r\nHost: highscore.de\r\n\r\n"));
		sock.async_read_some(asio::buffer(buffer), read_handler/*接收到数据后被调用*/);
	}
}

void resolve_handler(const asio::error_code &ec, asio::ip::tcp::resolver::iterator it)
{
	if (!ec)
	{
		sock.async_connect(*it, connect_handler/*在连接被建立后调用*/);
	}
}

///3
void write_handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
}

void accept_handler(const asio::error_code &ec)
{
	if (!ec)
	{
		asio::async_write(sock, asio::buffer(data), write_handler);//这个异步操作仅在缓冲区的所有字节都被发送后才结束
	}
}

///4
void timer(const asio::error_code& ec, asio::steady_timer* t)
{
	static unsigned i = 0;
	std::cout << "i = " << i << std::endl;
	if (i++ < 5)
	{
		t->expires_at(t->expiry() + std::chrono::seconds(1));
		t->async_wait(std::bind(timer, std::placeholders::_1, t));
	}
}

int main1()
{
	asio::steady_timer timer1(io_service, asio::chrono::seconds(5));
	asio::steady_timer timer2(io_service, asio::chrono::seconds(5));
	// strand提供串行执行, 能够保证线程安全, 同时被post或dispatch的方法, 不会被并发的执行. 
	// io_service不能保证线程安全
	//timer1.async_wait(handler);
	//timer2.async_wait(handler);
	timer1.async_wait(asio::bind_executor(io_strand, handler));
	timer2.async_wait(asio::bind_executor(io_strand, handler));
	///run()是阻塞的。
	///因此调用 run() 后程序执行会停止。 
	///具有讽刺意味的是，许多操作系统只是通过阻塞函数来支持异步操作。
	///如果在某个 asio::io_service 类型的对象之上调用 run() 方法，则相关联的句柄也会在同一个线程内被执行。
	///通过使用多线程，应用程序可以同时调用多个 run() 方法。
	///一旦某个异步操作结束，相应的 I/O 服务就将在这些线程中的某一个之中执行句柄。如果第二个操作在第一个操作之后很快也结束了，则 I/O 服务可以在另一个线程中执行句柄，而无需等待第一个句柄终止。
	std::thread th1(&run);
	std::thread th2(&run);
	if (th1.joinable())
		th1.join();
	if (th2.joinable())
		th2.join();
	///The asio library provides a guarantee that callback handlers will only be called from threads that are currently calling io_context::run().
	///Therefore unless the io_context::run() function is called the callback for the asynchronous wait completion will never be invoked.
	//io_service.run();

	return 0;
}

int main2()
{
	asio::ip::tcp::resolver::query query("www.highscore.de", "80");
	resolver.async_resolve(query, resolve_handler/*一旦域名解析成功或被某个错误中断被调用*/);
	io_service.run();

	return 0;
}

int main3()
{
	acceptor.listen();
	acceptor.async_accept(sock, accept_handler);
	io_service.run();

	return 0;
}

int main4()
{
	asio::steady_timer t(io_service, asio::chrono::seconds(1));
	t.async_wait(std::bind(timer, std::placeholders::_1, &t));
	io_service.run();

	return 0;
}

//int main()
//{
//	//main1();
//	//main2();
//	//main3();
//	//main4();
//}