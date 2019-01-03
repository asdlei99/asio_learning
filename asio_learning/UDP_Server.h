#pragma once

#include "pch.h"

using asio::ip::udp;

class udp_server
{
public:
	udp_server(asio::io_context& io_context)
		: m_socket(io_context, udp::endpoint(udp::v4(), 13))
	{
		start_receive();
	}
private:
	void start_receive()
	{
		static std::array<char, 1> recv_buffer;
		m_socket.async_receive_from(asio::buffer(recv_buffer), m_endpoint,
			std::bind(&udp_server::handle_receive, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
	void handle_receive(const asio::error_code &ec, std::size_t bytes_transferred)
	{
		if (!ec)
		{
			std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
			std::cout << "a client connected with udp." << std::endl;
			std::cout << "local endpoint : " << m_socket.local_endpoint().address() << std::endl;
			std::cout << "remote endpoint : " << m_endpoint.address() << std::endl;
			m_socket.async_send_to(asio::buffer(*message), m_endpoint,
				std::bind(&udp_server::handle_send, this, message, std::placeholders::_1, std::placeholders::_2));
			std::cout << make_daytime_string() << std::endl;
			start_receive();
		}
	}
	void handle_send(std::shared_ptr<std::string> /*message*/,
		const asio::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
		std::cout << "answer " << m_endpoint.address() << " succeed." << std::endl;
		std::cout << std::string(30, '*') << std::endl;
	}
	udp::socket m_socket;
	udp::endpoint m_endpoint;
};