/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:08:08 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/02 09:53:13 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConf.hpp"

// Funzione per rimuovere spazi iniziali e finali da una stringa
static std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first != std::string::npos)
	{
		size_t last = str.find_last_not_of(" \t\n\r\f\v");
		return str.substr(first, (last - first + 1));
	}
	return "";
}

static bool skipCommentedOrEmptyLines(std::string& line)
{
	size_t commentPos = line.find('#');
	if (commentPos != std::string::npos)
		line = line.substr(0, commentPos);
	line = trim(line);
	if (!line.empty() && line[0] != '#')
		return true;
	return false;
}


bool isValidKey(const std::string& key)
{
	const char *validKeysArray[] = {"listen", "serverName", "error_page", "location", "allow_methods", "root", "index", "host" , "autoindex", "client_max_body_size"};
	const int numvalidKeys = sizeof(validKeysArray) / sizeof(validKeysArray[0]);
	for(unsigned long i = 0; i < numvalidKeys; ++i)
		if(key == validKeysArray[i])
			return (true);
	return (false);
}


int IsValidServer(const ServerConfig& serverConfig, const LocationConfig& LocationConfig)
{
	if (serverConfig.port <= 0 || serverConfig.port >= 65536)
		return (0);
	if (serverConfig.host.empty())
		return (0);
	if (LocationConfig.root.empty() || serverConfig.index.empty())
		return (0);
	// if(chdir(LocationConfig.root.c_str()) == -1)
	// 	return (0);
	return (1);
}

void parseServerconf(const std::string& configfile, std::vector<ServerConfig>& serverConfigs)
{
	// Controllo che il file sia stato aperto correttamente
	std::ifstream file(configfile);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file" << std::endl;
		return;
	}
	// Leggo il file riga per riga e divido la riga in due parti, la prima parte è la chiave,
	// la seconda il valore usando come separatore il carattere ' '
	std::string line;
	while (std::getline(file, line))
	{
	if (!skipCommentedOrEmptyLines(line))
		continue;
	//std::cout << "Read line: " << line << std::endl;
	std::istringstream iss(line);
	std::string key;
	iss >> key;
	if (key == "server")
	{
		ServerConfig serverConfig;
		while (std::getline(file, line) && line != "}")
		{
			if (!skipCommentedOrEmptyLines(line))
				break;
			std::istringstream issBlock(line);
			std::string blockkey;
			issBlock >> blockkey;
			if (!isValidKey(blockkey) && blockkey != "}")
				std::cerr << "Unknown key: " << blockkey << std::endl;
			else
			{
				if (blockkey == "listen")
					issBlock >> serverConfig.port;
				else if (blockkey == "serverName")
					issBlock >> serverConfig.server_name;
				else if (blockkey == "host")
					issBlock >> serverConfig.host;
				else if (blockkey == "client_max_body_size")
					issBlock >> serverConfig.client_max_body_size;
				else if (blockkey == "root")
					issBlock >> serverConfig.root;
				else if (blockkey == "index")
					issBlock >> serverConfig.index;
				else if (blockkey == "error_page")
				{
					int errorCode;
					std::string errorPath;
					issBlock >> errorCode >> errorPath;
					serverConfig.errorPages.insert(std::make_pair(std::to_string(errorCode), errorPath));
				}
				else if (blockkey == "location")
				{
					if (!skipCommentedOrEmptyLines(line))
						break;
					LocationConfig locationConfig;
					issBlock >> locationConfig.locationPath;
					while (std::getline(file, line) && line != "}")
					{
						std::cout << "line: " << line << std::endl;
						if (!skipCommentedOrEmptyLines(line))
							continue;
						std::istringstream issLocation(line);
						std::string locationKey;
						issLocation >> locationKey;
						if (!isValidKey(locationKey) && locationKey != "}")
							std::cerr << "Unknown key in location block: " << locationKey << std::endl;
						else
						{
							if(locationKey == "autoindex")
								issLocation >> locationConfig.autoindex;
							else if (locationKey == "allow_methods")
							{
								std::string method;
								while (issLocation >> method)
									locationConfig.allow_methods.push_back(method);
							}
							else if (locationKey == "root")
								issLocation >> locationConfig.root;
							else if (locationKey == "index")
								issLocation >> locationConfig.index;
							else
								std::cerr << "Unknown key: " << locationKey << std::endl;
							if (line == "}")
								break;
						}
					}
					serverConfig.locations[locationConfig.locationPath] = locationConfig;
					}
				}
			}
			serverConfigs.push_back(serverConfig);
		}
	}
	file.close();
}

