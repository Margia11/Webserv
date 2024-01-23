/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:41 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/23 15:51:01 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainServer.hpp"

MainServer::MainServer(const std::vector<ServerConfig>& serverConfigs) : Server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	for (std::vector<ServerConfig>::const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it)
	{
		VirtualServer virtualServer(*it);
		virtualServers.push_back(virtualServer);
	}
}

void MainServer::Accepter()
{
	struct sockaddr_in address = getSocket()->getAddress();
	int addrlen = sizeof(address);
	newsocket = accept(getSocket()->getSocket(), (struct sockaddr *)&address,
			(socklen_t*)&addrlen);
	read(newsocket, buffer, 30000);
	//cerco un posto libero nella lista dei file descriptor
	//e lo preparo per la scrittura
	for (int i = 1; i < MAX_EVENTS; i++)
	{
		if (fds[i].fd == -1)
		{
			fds[i].fd = newsocket;
			fds[i].events = POLLOUT;
			break;
		}
	}
}

void MainServer::Handler()
{
	std::cout << buffer << std::endl;
}

void MainServer::Responder()
{
	const char *hello = "Hello from server";
	write(newsocket, hello, strlen(hello));
	close(newsocket);
}

void MainServer::launch()
{
	for(std::vector<VirtualServer>::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it)
		it->launch();
}

