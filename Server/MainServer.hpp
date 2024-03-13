/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amargiac <amargiac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:33 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/13 19:24:41 by amargiac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "utils.hpp"
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include "SimpleServer.hpp"
#include "../Parser/ParserRequest.hpp"
#include "../Response/GetResponse.hpp"
#include "../Response/PostResponse.hpp"
#include "../Response/DeleteResponse.hpp"

typedef std::map<int, ParserRequest>	clientHttpParserMap_t;

class MainServer
{
	private :
		std::map<int, Server> SimpleServers;
		clientHttpParserMap_t _clientHttpParserMap;
		ParserRequest _parserRequest;
		std::vector<pollfd>	_fds;
	public :
		MainServer(const std::string& config);
		~MainServer();
		void launch();
		void _handleConnections(int fd);
		void _handleRequest(std::vector<pollfd>::iterator it);
		void printServer();
		std::string readFromFd(int fd);
		void clearfds();
		void resetParserRequest(std::vector<pollfd>::iterator it);
		std::map<int, Server> getServers(void);
};

#endif
