/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amargiac <amargiac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:41:40 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/13 19:34:48 by amargiac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Networking/Networking.hpp"
#include "Server/MainServer.hpp"
#include "Parser/ParserConf.hpp"
#include "Server/VirtualServer.hpp"
#include <signal.h>

MainServer *closingRef;

static void closeServer(int sig)
{
	if(sig != SIGINT)
		return;
	std::cout << "Closing server" << std::endl;
	printFds();
	std::map<int, Server> servs = closingRef->getServers();
	for (std::map<int, Server>::iterator it = servs.begin(); it != servs.end(); it++)
		close(it->second.getSocket()->getSocket());
	printFds();
	closingRef->clearfds();
	exit(0);
}
int main(int argc, char **argv)
{
	std::string configfile = "conf/configfile.conf";
	if (argc > 1)
		configfile = argv[1];
	else
	{
		std::cout << "No config file specified"<< std::endl;
		return 1;
	}
	MainServer server(configfile);
	closingRef = &server;
	signal(SIGINT, closeServer);
	server.launch();
	return (0);
}
