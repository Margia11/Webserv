/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:33 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/07 12:07:06 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "SimpleServer.hpp"
// #include "VirtualServer.hpp"
#include "GetResponse.hpp"
#include "PostResponse.hpp"
#include "../Parser/ParserRequest.hpp"
#include "utils.hpp"
#include <stdio.h>
#include <string.h>
#include <poll.h>

typedef std::map<int, ParserRequest>	clientHttpParserMap_t;

class MainServer
{
	private :
		std::vector<pollfd>	_fds;
		std::map<int, Server> SimpleServers;
		clientHttpParserMap_t _clientHttpParserMap;
		ParserRequest _parserRequest;
	public :
		MainServer(const std::string& config);
		~MainServer();
		void launch();
		void _handleConnections(int fd);
		void _handleRequest(std::vector<pollfd>::iterator it);
		void printServer();
		void clearfds();

};

#endif
