/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/07 13:00:57 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleServer.hpp"

Server::Server()
{

}

//costruisce un server con host e porta forniti da configfile e aggiunge un virtualserver
Server::Server(ServerConfig config)
{
	HostPort = std::make_pair(config.host, config.port);
	this->addVirtualServer(config);
}


Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
	std::cout << "Server created" << std::endl;
}

//aggiunge un virtualserver(con tutte le info) al vettore di server
void Server::addVirtualServer(ServerConfig &config)
{
	servers.push_back(VirtualServer(config));
}

Server::~Server()
{
}

ListeningSocket *Server::getSocket()
{
	return (socket);
}

void Server::setSocket(ListeningSocket *socket)
{
	this->socket = socket;
}

int Server::getServerFd() const
{
	return ServerFd;
}

std::pair<std::string, int> Server::getHostPort() const
{
	return HostPort;
}

int Server::getPort() const
{
	return HostPort.second;
}

VirtualServer Server::getFirstVS()
{
	return this->servers[0];
}

