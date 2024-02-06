/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:32:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/06 12:14:43 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

// funzione che converte l'ip da stringa a byte order
static u_int32_t string_to_byte_order(const char *ip_addr) {
	int octets[4];
	size_t i = 0;
	int j = 0;
	int h = 0;
	char tmp[3];

	if (ip_addr == NULL)
		return 0;

	while (i < strlen(ip_addr) + 1)
	{
		if (ip_addr[i] == '.' || i == (strlen(ip_addr)))
		{
			tmp[j] = 0;
			octets[h] = std::atoi(tmp);
			h++;
			j = 0;
			i++;
			continue;
		}
		tmp[j] = ip_addr[i];
		i++;
		j++;
	}
	for (i = 0; i < 4; i++) {
		if (octets[i] < 0 || octets[i] > 255)
			return 0;
	}
	return ((u_int32_t)octets[0] << 24) | ((u_int32_t)octets[1] << 16) | ((u_int32_t)octets[2] << 8) | ((u_int32_t)octets[3]);
}

//fa il setup di ogni server con le info del configfile
VirtualServer::VirtualServer(const ServerConfig& config)
{
	this->server_name = config.server_name;
	this->root = config.root;
	this->index = config.index;
	this->errorPages = config.errorPages;
	this->client_max_body_size = config.client_max_body_size;
	for (std::map<std::string, LocationConfig>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++)
	{
		LocationInfo locationInfo;
		locationInfo.root = it->second.root;
		locationInfo.index = it->second.index;
		locationInfo.autoindex = it->second.autoindex;
		locationInfo.allow_methods = it->second.allow_methods;
		this->locations[it->first] = locationInfo;
	}
}

VirtualServer::~VirtualServer()
{
}

std::string const &VirtualServer::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::string const &VirtualServer::getServerName() const
{
	return (this->server_name);
}

std::string const &VirtualServer::getRoot() const
{
	return (this->root);
}

std::string const &VirtualServer::getIndex() const
{
	return (this->index);
}

std::map <std::string, std::string> const &VirtualServer::getErrorPages() const
{
	return (this->errorPages);
}

std::map <std::string, LocationInfo> const &VirtualServer::getLocations() const
{
	return (this->locations);
}
