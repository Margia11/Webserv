/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/20 16:37:44 by andreamargi      ###   ########.fr       */
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
	std::string CGI_requested = getCGIScript(uri);
	std::cout << "CGI_requested: " << CGI_requested << std::endl;
	if (!CGI_requested.empty())
	{
		uri = uri.substr(0, uri.find_last_of("/") + 1);
	}
	std::map<std::string, LocationInfo> locations = vs->getLocations();
	std::map<std::string, LocationInfo>::iterator l = locations.find(uri);

	if (l != locations.end())
	{
		std::cout << "Found location" << std::endl;
		//uploadPath = l->second.uploadPath;
		allowedMethods = l->second.allow_methods;
		//errorPages = l->second.errorPages;
		//root = l->second.root;
		//clientMaxBodySize = l->second.clientMaxBodySize;
	}
	if (!allowedMethods.empty() && find(allowedMethods.begin(), allowedMethods.end(), "GET") == allowedMethods.end())
		setStatusCode(405);
	// else if (request.getBody().size() > convertToBytes(clientMaxBodySize))
	//	setStatusCode(413);
	else if (l != locations.end() && !(l->second.cgi_path.empty()) && !CGI_requested.empty())
	{
		CGI cgi(parser, &(l->second));
		setStatusCode(200);
		setHeaders_CGI(*parser, cgi.CGI_Executer());
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
