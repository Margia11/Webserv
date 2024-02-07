/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:41:40 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/07 10:50:15 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Networking/Networking.hpp"
#include "Server/MainServer.hpp"
#include "Parser/ParserConf.hpp"
#include "Server/VirtualServer.hpp"

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
	server.launch();
	return (0);
}
