/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/21 11:26:09 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"
#include <cstring>
#include <cstdlib>
#include <sstream>

GetResponse::GetResponse() : Response()
{
}

GetResponse::~GetResponse()
{
}

std::string GetResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::string response;
	std::string root = vs->getRoot();
	std::string index = vs->getIndex().front();
	int x = isValidFile((root + "/" + index).c_str());
	std::vector<std::string> allowedMethods = vs->getAllowMethods();
	string uri = parser->getUri();
	if (uri.empty())
		uri = "/";
	std::cout << "uri: " << uri << std::endl;
	std::string file_requested = getFile(uri);
	std::cout << "CGI_requested: " << file_requested << std::endl;
	if (!file_requested.empty())
	{
		uri = uri.substr(0, uri.find_last_of("/") + 1);
	}
	std::map<std::string, LocationInfo> locations = vs->getLocations();
	std::map<std::string, LocationInfo>::iterator l = locations.find(uri);

	bool use_CGI = false;
	if (l != locations.end())
	{
		if (!l->second.cgi_path.empty())
		{
			DIR *d = opendir((root + l->second.cgi_path).c_str());
			if (d)
			{
				std::cout << "Dir opened: " << root + l->second.cgi_path << std::endl;
				dirent *tmp = readdir(d);
				while (tmp != nullptr)
				{
					std::string elem = tmp->d_name;
					std::cout << "elem: " << elem << std::endl;
					if (file_requested.compare(elem) == 0)
						use_CGI = true;
					tmp = readdir(d);
				}
				closedir(d);
			}
			else
			{
				std::cout << "CGI path not found" << std::endl;
				setStatusCode(404);
				std::string err = vs->getErrorPages().find(numberToString(getStatusCode()))->second;
				setHeaders(*parser, vs->getMimeTypes(), err);
				response = toString();
				return response;
			}
		}
		//uploadPath = l->second.uploadPath;
		allowedMethods = l->second.allow_methods;
		//errorPages = l->second.errorPages;
		//root = l->second.root;
		//clientMaxBodySize = l->second.clientMaxBodySize;
	}
	if (!allowedMethods.empty() && find(allowedMethods.begin(), allowedMethods.end(), "GET") == allowedMethods.end())
		setStatusCode(405);
	else if (l != locations.end() && use_CGI)
	{
		std::cout << "Handling with CGI" << std::endl;
		CGI cgi(parser, &(l->second), file_requested);
		std::cout << "CGI object created" << std::endl;
		setStatusCode(200);
		std::string buffer = cgi.CGI_Executer();
		setHeaders_CGI(*parser, buffer);
		response = toString_CGI();
		return response;
	}
	if (parser->path == "/" && x)
	{
		setStatusCode(200);
		setHeaders(*parser, vs->getMimeTypes(), root + "/" + index);
		response = toString();
	}
	else if (isValidFile((root + "/" + parser->path).c_str()))
	{
		setStatusCode(200);
		setHeaders(*parser, vs->getMimeTypes(), root + "/" + parser->path);
		response = toString();
	}
	else
	{
		std::string err = vs->getErrorPages().find("404")->second;
		setStatusCode(404);
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	return response;
}
