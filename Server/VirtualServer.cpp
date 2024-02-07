/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:32:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/07 10:52:00 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

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
