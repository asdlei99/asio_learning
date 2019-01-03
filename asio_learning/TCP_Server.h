#pragma once

#include "pch.h"

using asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
	tcp_connection(asio::io_context& io_context)
		: m_socket(io_context)
	{
	}
	tcp::socket& socket()
	{
		return m_socket;
	}
	void answer()
	{
		m_timestr = make_daytime_string();
		std::cout << "a client connected with tcp." << std::endl;
		std::cout << "local endpoint : " << m_socket.local_endpoint().address() << std::endl;
		std::cout << "remote endpoint : " << m_socket.remote_endpoint().address() << std::endl;
		//asio::buffer函数本身并不申请内存，只是提供了一个对现有内存的封装，大小是自动管理的。
		//所以这里m_timestr不能是局部对象了。
		asio::async_write(m_socket, asio::buffer(m_timestr),
			std::bind(&tcp_connection::handle_write, shared_from_this(),
				std::placeholders::_1, std::placeholders::_2));
		std::cout << make_daytime_string() << std::endl;
	}
private:
	void handle_write(const asio::error_code &ec, std::size_t bytes_transferred)
	{
		std::cout << "answer " << m_socket.remote_endpoint().address() << " succeed." << std::endl;
		std::cout << std::string(30, '*') << std::endl;
	}

private:
	tcp::socket m_socket;
	std::string m_timestr;
};

class tcp_server
{
public:
	tcp_server(asio::io_context& io_context)
		: m_acceptor(io_context, tcp::endpoint(tcp::v4(), 13))
	{
		start_accept();
	}
private:
	void start_accept()
	{
		std::shared_ptr<tcp_connection> new_connection(new tcp_connection(m_acceptor.get_executor().context()));
		m_acceptor.async_accept(new_connection->socket(),
			std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1));
	}
	void handle_accept(std::shared_ptr<tcp_connection> new_connection, const asio::error_code &ec)
	{
		if (!ec)
		{
			new_connection->answer();
		}
		this->start_accept();
	}

private:
	tcp::acceptor m_acceptor;
};
