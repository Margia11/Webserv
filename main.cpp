/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:41:40 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/23 15:45:57 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Networking/Networking.hpp"
#include "Server/MainServer.hpp"
#include "Parser/ParserConf.hpp"
#include "Server/VirtualServer.hpp"

int main(int argc, char **argv)
{
	std::vector<ServerConfig> serverConfigs;
	std::string configfile = "conf/configfile.conf";
	if (argc > 1)
		configfile = argv[1];
	else
	{
		std::cout << "No config file specified"<< std::endl;
		return 1;
	}
	parseServerconf(configfile, serverConfigs);
	MainServer server(serverConfigs);
	server.launch();
	return (0);
}
