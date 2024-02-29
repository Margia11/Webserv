/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:08:08 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/28 15:54:05 by andreamargi      ###   ########.fr       */
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

static bool checkandcutsemicolon(std::string& line)
{
	size_t pos = line.find_last_not_of(" \t\n\r\f\v");
	if (pos != std::string::npos)
	{
		if (line[pos] == '{' || line[pos] == '}')
			return true;
		if (line[pos] == ';')
		{
			line = line.substr(0, pos);
			//std::cout << "Info without semicolon: " << line << std::endl;
			return true;
		}
	}
	std::cerr << "Error: missing semicolon" << std::endl;
	return false;
}



bool isValidKey(const std::string& key)
{
	const char *validKeysArray[] = {"listen", "serverName", "error_page", "location", "allow_methods", "root", "index", "host" , "autoindex", "client_max_body_size", "client_body_temp_path", "CGI_path", "try_files"};
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
	if (LocationConfig.locationPath.empty() || serverConfig.index.empty())
		return (0);
	// if(chdir(LocationConfig.root.c_str()) == -1)
	// 	return (0);
	return (1);
}

static bool isValidMethod(const std::string& method)
{
	if (method.compare("GET") == 0 || method.compare("POST") == 0 || method.compare("DELETE")== 0 )
		return (1);
	return (0);
}

void parseServerconf(const std::string& configfile, std::vector<ServerConfig>& serverConfigs)
{
	// Controllo che il file sia stato aperto correttamente
	std::ifstream file;
	file.open(configfile.c_str());
	if (!file.is_open())
	{
		std::cerr << "Unable to open file" << std::endl;
		exit(1);
	}
	// Leggo il file riga per riga e divido la riga in due parti, la prima parte è la chiave,
	// la seconda il valore usando come separatore il carattere ' '
	std::string line;
	while (std::getline(file, line))
	{
	if (!skipCommentedOrEmptyLines(line))
		continue;
	if (!checkandcutsemicolon(line))
		exit(1);
	std::istringstream iss(line);
	std::string key;
	iss >> key;
	if (key == "server")
	{
		ServerConfig serverConfig;
		while (std::getline(file, line) && line != "}")
		{
			if (!skipCommentedOrEmptyLines(line))
				continue;
			if(!checkandcutsemicolon(line))
				exit(1);
			std::istringstream issBlock(line);
			std::string blockkey;
			issBlock >> blockkey;
			if (!isValidKey(blockkey) && blockkey != "}")
				std::cerr << "Unknown key: " << blockkey << std::endl;
			else
			{
				std::string tmp;
				if (blockkey == "listen")
					issBlock >> serverConfig.port;
				else if (blockkey == "host")
					issBlock >> serverConfig.host;
				else if (blockkey == "client_body_temp_path")
					issBlock >> serverConfig.uploadPath;
				else if (blockkey == "client_max_body_size")
					issBlock >> serverConfig.client_max_body_size;
				else if (blockkey == "root")
					issBlock >> serverConfig.root;
				else if (blockkey == "serverName")
				{
					while (issBlock >> tmp)
						serverConfig.server_name.push_back(tmp);
				}
				else if (blockkey == "index")
				{
					while (issBlock >> tmp)
						serverConfig.index.push_back(tmp);
				}
				else if (blockkey == "error_page")
				{
					std::vector<std::string> errorCodes;
					while (issBlock >> tmp)
						errorCodes.push_back(tmp);
					int errorCodesNum = errorCodes.size();
					std::string errorPath = errorCodes[errorCodesNum - 1];
					for (int i = 0; i < errorCodesNum - 1; i++)
						serverConfig.errorPages.insert(std::make_pair(errorCodes[i], errorPath));
				}
				else if (blockkey == "location")
				{
					if (!skipCommentedOrEmptyLines(line))
						break;
					if(!checkandcutsemicolon(line))
						continue;
					LocationConfig locationConfig;
					issBlock >> locationConfig.locationPath;
					while (std::getline(file, line) && trim(line) != "}")
					{
						if(!checkandcutsemicolon(line))
							continue;
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
							{
								std::string tmp;
								issLocation >> tmp;
								if (tmp == "on")
									locationConfig.autoindex = true;
								else if (tmp == "off")
									locationConfig.autoindex = false;
								else
								{
									std::cerr << "Invalid value for autoindex: " << tmp << std::endl;
									exit(1);
								}
							}
							else if (locationKey == "allow_methods")
							{
								std::string method;
								while (issLocation >> method)
								{
									if (isValidMethod(method))
										locationConfig.allow_methods.push_back(method);
									else
									{
										std::cerr << "Invalid method specified in allow_methods: " << method << std::endl;
										exit(1);
									}
								}
								if (locationConfig.allow_methods.empty()) {
									std::cerr << "Error: allow_methods cannot be empty" << std::endl;
									exit(1);
								}
							}
							else if (locationKey == "root")
								issLocation >> locationConfig.root;
							else if (locationKey == "client_max_body_size")
								issLocation >> locationConfig.client_max_body_size;
							else if (locationKey == "client_body_temp_path")
								issLocation >> locationConfig.uploadPath;
							else if (locationKey == "error_page")
							{
								std::vector<std::string> errorCodes;
								while (issLocation >> tmp)
									errorCodes.push_back(tmp);
								int errorCodesNum = errorCodes.size();
								std::string errorPath = errorCodes[errorCodesNum - 1];
								for (int i = 0; i < errorCodesNum - 1; i++)
									locationConfig.errorPages.insert(std::make_pair(errorCodes[i], errorPath));
							}
							else if (locationKey == "CGI_path")
								issLocation >> locationConfig.CGI_path;
							else if (locationKey == "try_files")
							{
								std::string files;
								while (issLocation >> files)
									locationConfig.try_files.push_back(files);
							}
							else if (locationKey == "index")
							{
								std::string index;
								while (issLocation >> index)
									locationConfig.index.push_back(index);
							}
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
			if(IsValidServer(serverConfig, serverConfig.locations.begin()->second))
				serverConfigs.push_back(serverConfig);
			else
			{
				std::cerr << "Invalid server configuration" << std::endl;
				exit(1);
			}
		}
	}
	file.close();
}

void printServerConfigs(const std::vector<ServerConfig>& serverConfigs)
{
	for (std::vector<ServerConfig>::const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); it++)
	{
		std::cout << "Server: " << std::endl;
		std::cout << "Port: " << it->port << std::endl;
		std::cout << "Host: " << it->host << std::endl;
		std::cout << "Root: " << it->root << std::endl;
		std::cout << "UploadPath: " << it->uploadPath << std::endl;
		std::cout << "Client_max_body_size: " << it->client_max_body_size << std::endl;
		std::cout << "ServerName: ";
		for (std::vector<std::string>::const_iterator it2 = it->server_name.begin(); it2 != it->server_name.end(); it2++)
			std::cout << *it2 << " ";
		std::cout << std::endl;
		std::cout << "Index: ";
		for (std::vector<std::string>::const_iterator it2 = it->index.begin(); it2 != it->index.end(); it2++)
			std::cout << *it2 << " ";
		std::cout << std::endl;
		std::cout << "ErrorPages: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it2 = it->errorPages.begin(); it2 != it->errorPages.end(); it2++)
			std::cout << it2->first << " " << it2->second << std::endl;
	}
}
