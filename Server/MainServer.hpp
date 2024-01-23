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
#include <poll.h>

class MainServer : public Server
{
	private :
		char buffer[30000];
		int newsocket;
		static const int MAX_EVENTS = 10;
		struct pollfd fds[MAX_EVENTS];
		void Accepter();
		void Handler();
		void Responder();
		std::vector<VirtualServer> virtualServers;
	public :
		MainServer(const std::vector<ServerConfig>& serverConfigs);
		void launch();
};

#endif
