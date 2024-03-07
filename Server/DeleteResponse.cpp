/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 14:30:16 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/29 12:35:16 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteResponse.hpp"

DeleteResponse::DeleteResponse()
{
}

DeleteResponse::~DeleteResponse()
{
}


std::string DeleteResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::string response;
	std::string uploadPath = vs->getUploadPath();
	std::vector<std::string> allowedMethods = vs->getAllowMethods();
	std::map<std::string, std::string> errorPages = vs->getErrorPages();
	string root = vs->getRoot();
	string uri = parser->getUri();
	if (uri.empty())
		uri = "/";
	std::string file_requested = getFile(uri);
	if (!file_requested.empty())
		uri = uri.substr(0, uri.find_last_of("/") + 1);
	std::map<std::string, LocationInfo> locations = vs->getLocations();
	std::map<std::string, LocationInfo>::iterator l = locations.find(uri);
	if (l != locations.end())
	{
		errorPages = l->second.errorPages;
		allowedMethods = l->second.allow_methods;
	}
	std::vector<std::string>::iterator it = allowedMethods.begin();
	while (it != allowedMethods.end())
	{
		if (*it == "DELETE")
			break;
		std::cout << "it: " << *it;
		it++;
	}
	// std::cout << "uri: " << root + uri + file_requested << std::endl;
	if (!allowedMethods.empty() && it == allowedMethods.end())
		setStatusCode(405);
	else if(remove((root + uri + file_requested).c_str()) != 0)
		setStatusCode(404);
	else
		setStatusCode(200);
	if (getStatusCode() == 200)
	{
		setHeaders(*parser, vs->getMimeTypes(), "./web/html/success_delete.html");
		response = toString();
	}
	else
	{
		std::string err = errorPages.find(numberToString(getStatusCode()))->second;
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	return response;
}
