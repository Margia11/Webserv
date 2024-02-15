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
	this->uploadPath = config.uploadPath;
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
	_setMimeTypes();
}

void VirtualServer::printMimeType()
{
	for (std::map<std::string, std::string>::iterator it = _mimeTypes.begin(); it != _mimeTypes.end(); it++)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
}

std::map<std::string, std::string> const &VirtualServer::getMimeTypes() const
{
	return (_mimeTypes);
}

void VirtualServer::_setMimeTypes()
{
	_mimeTypes[".bmp"] = "image/bmp";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".epub"] = "application/epub+zip";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".html"] = "text/html";
    _mimeTypes[".ico"] = "image/vnd.microsoft.icon";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".js"] = "text/javascript";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mpeg"] = "video/mpeg";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".php"] = "application/x-httpd-php";
	_mimeTypes[".svg"] = "image/svg+xml";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".weba"] = "audio/webm";
	_mimeTypes[".webm"] = "video/webm";
    _mimeTypes[".webp"] = "image/webp";
    _mimeTypes[".mov"] = "video/quicktime";
    _mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".php"] = "application/x-httpd-php";
}

VirtualServer::~VirtualServer()
{
}

std::string const &VirtualServer::getUploadPath() const
{
	return (this->uploadPath);
}

std::string const &VirtualServer::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::vector<std::string> const &VirtualServer::getServerName() const
{
	return (this->server_name);
}

std::string const &VirtualServer::getRoot() const
{
	return (this->root);
}

std::vector<std::string> const &VirtualServer::getIndex() const
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

void VirtualServer::printVServer()
{
	std::cout << "Server ok" << std::endl;
}
