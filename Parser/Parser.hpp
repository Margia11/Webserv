/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:55:49 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/18 15:12:10 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

struct LocationConfig
{
	std::string locationPath;
	std::vector<std::string> allow_methods;
	std::string root;
	std::string index;
};

struct ServerConfig
{
	int port;
	std::string serverName;
	std::map<std::string, std::string> errorPages;
	std::map<std::string, LocationConfig> locations;
	std::string host;
};

void parseServerconf(const std::string& configfile, ServerConfig& serverConfig);
// void printServerConfig(const ServerConfig& serverConfig);

#endif
