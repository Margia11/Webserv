/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:41 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/15 16:58:10 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainServer.hpp"

MainServer::MainServer(const std::string& config) : _parserRequest()
{
	std::vector<ServerConfig> serverConfigs;
	parseServerconf(config, serverConfigs);
	printServerConfigs(serverConfigs);
	for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
	{
		if (SimpleServers.find(it->port) != SimpleServers.end())
			SimpleServers[it->port].addVirtualServer(*it);
		else
			SimpleServers[it->port] = Server(*it);
	}
	for (std::map<int, Server>::iterator it = SimpleServers.begin(); it != SimpleServers.end(); ++it)
	{
		u_long interface = string_to_byte_order(it->second.getHostPort().first.c_str());
		ListeningSocket *tmpsocket = new ListeningSocket(AF_INET, SOCK_STREAM, 0, it->first, interface, 10);
		it->second.setSocket(tmpsocket);
		cout << "Server on port " << it->first << " created" << endl;
		struct pollfd serverPollFd_ = {};
		serverPollFd_.fd = tmpsocket->getSocket();
		serverPollFd_.events = POLLIN;
		_fds.push_back(serverPollFd_);
	}
}

MainServer::~MainServer()
{
	std::cout << "Closing server" << std::endl;
	for (std::map<int, Server>::iterator it = SimpleServers.begin(); it != SimpleServers.end(); ++it)
		close(it->second.getSocket()->getSocket());
	clearfds();
}

void MainServer::launch()
{
	while (true)
	{
		int ready = poll(_fds.data(), _fds.size(), 10000); //50 millisecondi di timeout
		if (ready == -1)
		{
			std::cerr << "poll failed" << std::endl;
			break;
		}
		if (ready == 0) //nessun evento pronto
			continue;
		for(size_t i = 0; i < _fds.size(); i++)
		{
			if ((_fds[i].revents & POLLIN) || (_fds[i].revents & POLLOUT))
			{
				if (i < SimpleServers.size())
					_handleConnections(_fds[i].fd);
				else
				{
					_handleRequest(_fds.begin() + i);
					if (_fds[i].fd == -1) {
						_fds.erase(_fds.begin() + i);
						i--;
					}
				}
			}
		}
	}
}

static void _error(const std::string &msg, int err)
{
	std::string errorMsg = msg + (!err ? "." : (std::string(" | ") + strerror(errno)));
	std::cout << errorMsg << std::endl;
	exit(1);
}

std::string MainServer::readFromFd(int fd)
{
	char buffer[32000];
	bzero(buffer, 32000);
	int byte_read = read(fd, buffer, 32000);
	if (byte_read < 0)
	{
		perror("read");
		exit(1);
	}
	else if (byte_read == 0)
	{
		std::cout << "Client closed connection" << std::endl;
		close(fd);
		return "";
	}
	return std::string(buffer, byte_read);
}

void MainServer::_handleRequest(std::vector<pollfd>::iterator it)
{
	std::string buffer = readFromFd(it->fd);
	if (buffer.empty())
		return;
	std::cout << "Received request:\n";
	std::cout << buffer << std::endl;
	_clientHttpParserMap[it->fd].readRequest(buffer);
	std::string tmp = _clientHttpParserMap[it->fd].getHost();
	Server server = SimpleServers[toHostPort(tmp).second];
	//std::cout << "simple servers: " << this->SimpleServers.size() << std::endl;
	//std::cout << "virtual servers for this simple server: " << server.getVirtualServers().size() << std::endl;
	VirtualServer vs = server.getFirstVS();
	GetResponse getResponse;
	PostResponse postResponse;
	std::string answer;
	std::cout << "Answering request" << std::endl;
	if (_clientHttpParserMap[it->fd].method == "GET")
		answer = getResponse.answer(&(_clientHttpParserMap[it->fd]), &vs);
	else if (_clientHttpParserMap[it->fd].method == "POST")
		answer = postResponse.answer(&(_clientHttpParserMap[it->fd]), &vs);
	else if (_clientHttpParserMap[it->fd].method == "DELETE")
		std::cout << "DELETE" << std::endl;
	else
	{
		send(it->fd, "HTTP/1.1 405 Method Not Allowed\r\n\r\n", 36, 0);
		return;
	}
	send(it->fd, answer.c_str(), answer.size(), 0);
	std::cout << "Response:" << std::endl;
	std::cout << answer << std::endl;
	answer.clear();
}

void MainServer::_handleConnections(int fd)
{
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	struct sockaddr_in sockAddrIn_ = {};
	socklen_t clientLen_ = sizeof(sockAddrIn_);
	int clientFd_ = accept(fd, (struct sockaddr*)&sockAddrIn_, &clientLen_);
	if (clientFd_ == -1)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
		{
			cerr << "EWOULDBLOCK" << endl;
			return;
		}
		else
			_error("accept", 1);
	}

	if (setsockopt(clientFd_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0)
	{
		std::cerr << "Error setting timeout\n";
		exit(1);
	}

	if (setsockopt(clientFd_, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) < 0)
	{
		std::cerr << "Error setting timeout\n";
		exit(1);
	}
	struct pollfd clientPollFd_ = {};
	clientPollFd_.fd = clientFd_;
	clientPollFd_.events = POLLIN;
	_fds.push_back(clientPollFd_);
	_clientHttpParserMap[clientFd_] = ParserRequest();
}

void MainServer::clearfds()
{
	std::vector<pollfd>::iterator iter_ = _fds.begin();
	std::vector<pollfd>::iterator iterEnd_ = _fds.end();
	for(; iter_ != iterEnd_; iter_++)
		close(iter_->fd);
	_fds.clear();
}


// void MainServer::printServer()
// {
// 	for (std::map<int, VirtualServer>::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it)
// 	{
// 		std::cout << "Server name: " << it->second.getServerName() << std::endl;
// 		std::cout << "Port: " << it->second.getPort() << std::endl;
// 		std::cout << "Host: " << it->second.getHost() << std::endl;
// 		std::cout << "Root: " << it->second.getRoot() << std::endl;
// 		std::cout << "Index: " << it->second.getIndex() << std::endl;
// 		std::cout << "Error pages: " << std::endl;
// 		std::map<std::string, std::string> errorPages = it->second.getErrorPages();
// 		for (std::map<std::string, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it)
// 			std::cout << it->first << " " << it->second << std::endl;
// 		std::cout << "Locations: " << std::endl;
// 		std::map<std::string, LocationInfo> locations = it->second.getLocations();
// 		for (std::map<std::string, LocationInfo>::iterator it = locations.begin(); it != locations.end(); ++it)
// 			std::cout << it->first << " " << it->second.root << " " << it->second.index << " " << it->second.autoindex << std::endl;
// 		std::cout << "Client max body size: " << it->second.getClientMaxBodySize() << std::endl;
// 	}
// }
