/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:11 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/07 11:21:21 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include <vector>
#include <unistd.h>
#include "../Networking/Networking.hpp"
#include "../Parser/ParserConf.hpp"
#include "VirtualServer.hpp"

#define BUFFER_SIZE 30000

class Server
{
	private :
		std::vector<VirtualServer> servers;
		std::pair<std::string, int>	HostPort;
		int ServerFd;
		ListeningSocket *socket;
	public :
		Server(ServerConfig config);
		Server();
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		~Server();
		void addVirtualServer(ServerConfig &config);
		ListeningSocket *getSocket();
		int	getServerFd() const;
		std::pair<std::string, int>getHostPort() const;
		int	getPort() const;
		void setSocket(ListeningSocket *socket);
		VirtualServer getVSfromName(const string& serverName);
		std::vector<VirtualServer> getVirtualServers();
		void printServer();
};

#endif
