/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:11 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/06 11:34:33 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include "../Networking/Networking.hpp"
#include <unistd.h>
#include "../Parser/ParserConf.hpp"
#include <vector>
#include "VirtualServer.hpp"

#define BUFFER_SIZE 30000

class Server
{
	private :
		std::vector<VirtualServer> servers;
		std::pair<std::string, int>	HostPort;
		int ServerFd;
		ListeningSocket *socket;
		virtual void Accepter() = 0;
		virtual void Handler() = 0;
		virtual void Responder() = 0;
	public :
		Server(ServerConfig config);
		Server();
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		~Server();
		void Server::addVirtualServer(ServerConfig &config);
		virtual void launch() = 0;
		ListeningSocket *getSocket();
		int	getServerFd() const;
		std::pair<std::string, int>getHostPort() const;
		int	getPort() const;
};

#endif
