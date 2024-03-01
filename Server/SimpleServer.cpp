/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/01 11:07:53 by andreamargi      ###   ########.fr       */
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

std::vector<VirtualServer> Server::getVirtualServers()
{
	return servers;
}

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
	std::cout << "Server created" << std::endl;
}

//aggiunge un virtualserver(con tutte le info) al vettore di server
void Server::addVirtualServer(ServerConfig &config)
{
	VirtualServer newVS(config);
	servers.push_back(newVS);
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

VirtualServer Server::getVSfromName(const string& serverName)
{
	std::vector<VirtualServer>::iterator it = servers.begin();
	while (it != servers.end())
	{
		std::vector<std::string>::const_iterator it_sn = (*it).getServerName().begin();
		while (it_sn != (*it).getServerName().end())
		{
			if ((*it_sn).compare(serverName) == 0)
				return (*it);
			it_sn++;
		}
		it++;
	}
	return (*(servers.begin()));
}

void Server::printServer()
{
	std::cout << "======= Server =======" << std::endl;
	std::cout << "Host: " << HostPort.first << std::endl;
	std::cout << "Port: " << HostPort.second << std::endl;
	std::cout << "Virtual Servers: " << servers.size() << std::endl;
	std::vector<VirtualServer>::iterator it = servers.begin();
	while (it != servers.end())
	{
		it->printVServer();
		it++;
	}
	std::cout << "======================" << std::endl;
}
