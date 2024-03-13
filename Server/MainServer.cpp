/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amargiac <amargiac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:41 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/13 19:22:17 by amargiac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainServer.hpp"

MainServer::MainServer(const std::string& config) : _parserRequest()
{
	std::vector<ServerConfig> serverConfigs;
	parseServerconf(config, serverConfigs);
	//printServerConfigs(serverConfigs);
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
		//cout << "Server on port " << it->first << " created" << endl;
		struct pollfd serverPollFd_ = {};
		serverPollFd_.fd = tmpsocket->getSocket();
		serverPollFd_.events = POLLIN;
		_fds.push_back(serverPollFd_);
		//it->second.printServer();
	}
}

std::map<int, Server> MainServer::getServers(void)
{
	return this->SimpleServers;
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
		int ready = poll(_fds.data(), _fds.size(), 0);
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
	std::string ret("");
	int byte_read = 0;
	// int total_byte_read = 0;
	char buffer[32001];
	while (true)
	{
		memset(buffer, 0, 32000);
		byte_read = read(fd, buffer, 32000);
		// std::cout << "Read " << byte_read << std::endl;
		//if (byte_read < 0 && ret.empty()) -> perror
		if (byte_read < 0)
		{
			perror("read");
			exit(1);
		}
		else if (byte_read == 0)
		{
			return (ret);
		}
		else if (byte_read < 32000)
		{
			buffer[byte_read] = '\0';
			ret.append(buffer, byte_read);
			break;
		}
		buffer[byte_read] = '\0';
		ret.append(buffer, byte_read);
		//std::cout << "Read " << byte_read << std::endl;
		//usleep(150);
		//std::cout << "Read " << ret << std::endl;
		// total_byte_read += byte_read;
	}
	return (ret);
}

void MainServer::resetParserRequest(std::vector<pollfd>::iterator it)
{
	std::map<int, ParserRequest>::iterator it_ = _clientHttpParserMap.find(it->fd);
	if (it_ != _clientHttpParserMap.end())
		_clientHttpParserMap.erase(it_);
	_clientHttpParserMap[it->fd] = ParserRequest();
}

void MainServer::_handleRequest(std::vector<pollfd>::iterator it)
{
	std::string answer;
	// std::cout << "New request issued from " << it->fd << std::endl;
	std::string buffer = readFromFd(it->fd);
	// std::cout << "Buffer: " << buffer.size() << std::endl;
	if (buffer.empty())
	{
		std::cout << "Client closed connection" << std::endl;
		close(it->fd);
		it->fd = -1;
		return;
	}
	// std::cout << "Buffer read" << std::endl;
/* 	std::cout << "Received request:\n";
	std::cout << buffer << std::endl; */
	bool validRequest = _clientHttpParserMap[it->fd].readRequest(buffer);
	if (!validRequest)
	{
		std::cerr << "Invalid request" << std::endl;
		answer = "HTTP/1.1 400 Bad Request\r\n\r\n";
		long int dataSent = send(it->fd, answer.data(), answer.size(), 0);
		if (dataSent < 0)
		{
			perror("send");
			exit(1);
		}
		else if (dataSent == 0)
		{
			std::cout << "Client closed connection" << std::endl;
			close(it->fd);
			it->fd = -1;
			return;
		}
		answer.clear();
		/* close(it->fd);
		it->fd = -1; */
		return;
	}
	bool validBody = _clientHttpParserMap[it->fd].checkBody();
	while (!validBody)
	{
		buffer.clear();
		// std::cout << "Reading rest of the body" << std::endl;
		buffer = readFromFd(it->fd);
		if (buffer.empty())
		{
			std::cout << "Client closed connection" << std::endl;
			close(it->fd);
			it->fd = -1;
			return;
		}
		_clientHttpParserMap[it->fd].setRequestBody(_clientHttpParserMap[it->fd].getRequestBody() + buffer);
		// std::cout << "checking body" << std::endl;
		validBody = _clientHttpParserMap[it->fd].checkBody();
	}
	std::string tmp = _clientHttpParserMap[it->fd].getHost();
	Server server = SimpleServers[toHostPort(tmp).second];
	//std::cout << "simple servers: " << this->SimpleServers.size() << std::endl;
	//std::cout << "virtual servers for this simple server: " << server.getVirtualServers().size() << std::endl;
	VirtualServer vs = server.getVSfromName(_clientHttpParserMap[it->fd].getHost());
	GetResponse getResponse;
	PostResponse postResponse;
	DeleteResponse deleteResponse;
	// std::cout << "Answering request" << std::endl;
	if (_clientHttpParserMap[it->fd].method == "GET")
		answer = getResponse.answer(&(_clientHttpParserMap[it->fd]), &vs);
	else if (_clientHttpParserMap[it->fd].method == "POST")
		answer = postResponse.answer(&(_clientHttpParserMap[it->fd]), &vs);
	else if (_clientHttpParserMap[it->fd].method == "DELETE")
		answer = deleteResponse.answer(&(_clientHttpParserMap[it->fd]), &vs);
	else
		answer = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
	//std::cout << "Response: " << answer << std::endl;
	long int dataSent;
	while (answer.size() > 32000)
	{
		std::string temp = answer.substr(0, 32000);
		dataSent = send(it->fd, temp.data(), temp.size(), 0);
		if (dataSent < 0)
		{
			perror("send");
			exit(1);
		}
		else if (dataSent == 0)
		{
			std::cout << "Client closed connection" << std::endl;
			close(it->fd);
			it->fd = -1;
			return;
		}
		temp.clear();
		answer = answer.substr(dataSent);
	}
	dataSent = send(it->fd, answer.data(), answer.size(), 0);
	if (dataSent < 0)
	{
		perror("send");
		exit(1);
	}
	else if (dataSent == 0)
	{
		std::cout << "Client closed connection" << std::endl;
		close(it->fd);
		it->fd = -1;
		return;
	}
	answer.clear();
	resetParserRequest(it);
	//reset parser _clientHttpParserMap[it->fd]
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
	struct pollfd clientPollFd_ = {};
	clientPollFd_.fd = clientFd_;
	clientPollFd_.events = POLLIN;
	_fds.push_back(clientPollFd_);
	// std::cout << "New connection issued from " << clientFd_ << std::endl;
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

