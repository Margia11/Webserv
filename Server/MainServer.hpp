/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:33 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/23 15:47:18 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "SimpleServer.hpp"
#include "VirtualServer.hpp"
#include <stdio.h>
#include <string.h>
#include <poll.h>

typedef std::map<int, ParserRequest>	clientHttpParserMap_t;

class MainServer
{
	private :
		std::vector<pollfd>	_fds;
		std::map<int,VirtualServer> virtualServers;
		clientHttpParserMap_t _clientHttpParserMap;
		char *buffer;
	public :
		MainServer(const std::vector<ServerConfig>& serverConfigs);
		void launch();
		void _handleConnections(int fd);
		void _handleRequest(int fd);
		void printServer();
};

#endif
