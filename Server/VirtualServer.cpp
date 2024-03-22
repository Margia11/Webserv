/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:32:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/22 15:47:00 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

//fa il setup di ogni server con le info del configfile
VirtualServer::VirtualServer(const ServerConfig& config)
{
	this->server_name = config.server_name;
	this->root = config.root;
	std::vector<std::string> tmp = config.index;
	std::vector<std::string>::iterator it = tmp.begin();
	while (it != tmp.end())
	{
		if (isValidFile((this->root + "/" + *it).c_str()))
			this->index.push_back(*it);
		it++;
	}
	this->autoindex = config.autoindex;
	this->uploadPath = config.uploadPath;
	this->errorPages = config.errorPages;
	this->client_max_body_size = config.client_max_body_size;
	for (std::map<std::string, LocationConfig>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++)
	{
		LocationInfo locationInfo;
		locationInfo.root = it->second.root;
		locationInfo.path = it->second.locationPath;
		locationInfo.index = it->second.index;
		locationInfo.autoindex = it->second.autoindex;
		locationInfo.allow_methods = it->second.allow_methods;
		locationInfo.cgi_path = it->second.CGI_path;
		locationInfo.try_files = it->second.try_files;
		locationInfo.client_max_body_size = it->second.client_max_body_size;
		locationInfo.errorPages = it->second.errorPages;
		locationInfo.uploadPath = it->second.uploadPath;
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
	_mimeTypes[".jpeg"] = "img/jpeg";
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

/* LocationInfo* VirtualServer::matchLocation(std::map<std::string, LocationInfo> locations, std::string uri)
{
	return &(locations[uri]);
} */

std::string const &VirtualServer::getUploadPath() const
{
	return (this->uploadPath);
}

long unsigned int VirtualServer::getClientMaxBodySize() const
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

bool VirtualServer::getAutoindex() const
{
	return (this->autoindex);
}

std::map <std::string, std::string> const &VirtualServer::getErrorPages() const
{
	return (this->errorPages);
}

std::map <std::string, LocationInfo> const &VirtualServer::getLocations() const
{
	return (this->locations);
}

std::vector <std::string> const &VirtualServer::getAllowMethods() const
{
	return (this->allow_methods);
}

void VirtualServer::printVServer()
{
	std::cout << "server_name: ";
	for (std::vector<std::string>::iterator it = this->server_name.begin(); it != this->server_name.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "root: " << this->root << std::endl;
	std::cout << "index: ";
	for (std::vector<std::string>::iterator it = this->index.begin(); it != this->index.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "autoindex: " << this->autoindex << std::endl;
	std::cout << "uploadPath: " << this->uploadPath << std::endl;
	std::cout << "client_max_body_size: " << this->client_max_body_size << std::endl;
	std::cout << "errorPages: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	std::cout << "locations: " << std::endl;
	for (std::map<std::string, LocationInfo>::iterator it = this->locations.begin(); it != this->locations.end(); it++)
	{
		std::cout << "	location path: " << it->first << std::endl;
		std::cout << "	location root: " << it->second.root << std::endl;
		std::cout << "	location index: ";
		for (std::vector<std::string>::iterator it2 = it->second.index.begin(); it2 != it->second.index.end(); it2++)
			std::cout << *it2 << " ";
		std::cout << std::endl;
		std::cout << "	location autoindex: " << it->second.autoindex << std::endl;
		std::cout << "	location uploadPath: " << it->second.uploadPath << std::endl;
		std::cout << "	location client_max_body_size: " << it->second.client_max_body_size << std::endl;
		std::cout << "	location errorPages: " << std::endl;
		for (std::map<std::string, std::string>::iterator it3 = it->second.errorPages.begin(); it3 != it->second.errorPages.end(); it3++)
			std::cout << it3->first << " " << it3->second << std::endl;
		std::cout << "	location allow_methods: ";
		for (std::vector<std::string>::iterator it4 = it->second.allow_methods.begin(); it4 != it->second.allow_methods.end(); it4++)
			std::cout << *it4 << " ";
		std::cout << std::endl;
	}
}

void VirtualServer::printLocations(VirtualServer *vs)
{
    std::cout << "locations: " << std::endl;
    const std::map<std::string, LocationInfo> &locations = vs->getLocations();
    for (std::map<std::string, LocationInfo>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        std::cout << it->first << std::endl;
        std::cout << it->second.root << std::endl;
    }
}

