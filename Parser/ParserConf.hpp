/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:55:49 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/29 12:00:47 by andreamargi      ###   ########.fr       */
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
#include <cstring>
#include <cstdlib>

struct LocationConfig
{
	std::string locationPath;
	std::string root;
	std::string CGI_path;
	std::string uploadPath;
	std::vector<std::string> allow_methods;
	std::vector<std::string> index;
	std::vector<std::string> try_files;
	std::map<std::string, std::string> errorPages;
	bool autoindex;
	long unsigned int client_max_body_size;
};

struct ServerConfig
{
	long unsigned int client_max_body_size;
	int port;
	std::vector<std::string> index;
	std::string uploadPath;
	bool autoindex;
	std::string root;
	std::string host;
	std::vector<std::string> server_name;
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
void printServerConfigs(const std::vector<ServerConfig>& serverConfigs);

#endif
