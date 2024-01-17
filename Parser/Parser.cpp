/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 15:08:08 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/08 18:08:52 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

static bool isCommentOrEmpty(const std::string& line)
{
	return line.empty() || line[0] == '#';
}


bool isValidKey(const std::string& key)
{
	const char *validKeysArray[] = {"listen", "serverName", "error_page", "location", "allow_methods", "root", "index"};
	const int numvalidKeys = sizeof(validKeysArray) / sizeof(validKeysArray[0]);
	for(unsigned long i = 0; i < numvalidKeys; ++i)
		if(key == validKeysArray[i])
			return (true);
	return (false);
}

void parseServerconf(const std::string& configfile, ServerConfig& serverConfig)
{
	//controllo che il file sia stato aperto correttamente
	std::ifstream file(configfile);
	if(!file.is_open())
	{
		std::cerr << "Unable to open file" << std::endl;
		return ;
	}
	//leggo il file riga per riga e divido la riga in due parti, la prima parte è la chiave, la seconda il valore usando come separatore il carattere ' '
	std::string line;
	// bool insideLocationBlock = false;
	while(std::getline(file, line))
	{
		if(isCommentOrEmpty(line))
			continue;
		std::istringstream iss(line);
		std::string key;
		iss >> key;
		if(key == "server")
		{
			while(std::getline(file, line) && line != "}")
			{
				std::istringstream issBlock(line);
				std::string blockkey;
				issBlock >> blockkey;
				if (!isValidKey(blockkey))
				{
					if(blockkey == "}")
					{
						// insideLocationBlock = false;
						continue;
					}
					std::cerr << "Unknown key: " << blockkey << std::endl;
				}
				else
				{
					if(blockkey == "listen")
						issBlock >> serverConfig.port;
					else if(blockkey == "serverName")
						issBlock >> serverConfig.serverName;
					else if(blockkey == "errorPage")
					{
						int errorCode;
						std::string errorPath;
						iss >> errorCode >> errorPath;
						serverConfig.errorPages.insert(std::make_pair(std::to_string(errorCode), errorPath));
					}
					else if(blockkey == "location")
					{
						LocationConfig locationConfig;
						issBlock >> locationConfig.locationPath;
						while (std::getline(file, line) && line != "}")
						{
							std::istringstream issLocation(line);
							std::string locationKey;
							issLocation >> locationKey;
							if (!isValidKey(locationKey))
								std::cerr << "Unknown key in location block: " << locationKey << std::endl;
							else
							{
								if(locationKey == "allow_methods")
								{
									std::string method;
									while(issLocation >> method)
										locationConfig.allow_methods.push_back(method);
								}
								else if(locationKey == "root")
									issLocation >> locationConfig.root;
								else if(locationKey == "index")
									issLocation >> locationConfig.index;
								else
									std::cerr << "Unknown key: " << locationKey << std::endl;
							}
						}
						serverConfig.locations[locationConfig.locationPath] = locationConfig;
					}
				}
			}
		}
	}
	file.close();
}
