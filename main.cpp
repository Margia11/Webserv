/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:41:40 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/08 16:37:27 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Networking/Networking.hpp"
#include "Server/TestServer.hpp"
#include "Parser/Parser.hpp"

int main(int argc, char **argv)
{
	// TestServer server;
	std::string configfile = "conf/configfile.conf";
	if (argc > 1)
		configfile = argv[1];
	else
	{
		std::cout << "No config file specified"<< std::endl;
		return 1;
	}
	ServerConfig serverConfig;
	parseServerconf(configfile, serverConfig);
	// printServerConfig(serverConfig);
	return (0);
}
