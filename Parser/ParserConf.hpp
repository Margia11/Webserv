/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:55:49 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/23 11:33:09 by andreamargi      ###   ########.fr       */
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
#include <unistd.h>

struct LocationConfig
{
	std::string locationPath;
	std::vector<std::string> allow_methods;
	std::string root;
	std::string autoindex;
};

struct ServerConfig
{
	int maxBodySize;
	int port;
	std::string index;
	std::string root;
	std::string host;
	std::string serverName;
	std::map<std::string, std::string> errorPages;
	std::map<std::string, LocationConfig> locations;
};

class InvalidServerException : public std::exception
{
	virtual const char* what() const throw()
	{
		return ("Invalid server configuration");
	}
};

void parseServerconf(const std::string& configfile, std::vector<ServerConfig>& serverConfigs);

#endif
