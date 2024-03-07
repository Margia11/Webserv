/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/01 11:54:36 by andreamargi      ###   ########.fr       */
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

std::string GetResponse::generateAutoindex(std::string root, std::string uri)
{
	std::string response;
	DIR *d = opendir((root + uri).c_str());
	if (d)
	{
		dirent *tmp = readdir(d);
		while (tmp != NULL)
		{
			if (tmp->d_name[0] != '.')
			{
				response += "<a href=\"" + uri + tmp->d_name + "\">" + tmp->d_name + "</a><br>";
			}
			tmp = readdir(d);
		}
		closedir(d);
	}
	return response;
}


std::string GetResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::string response;
	bool autoindex = vs->getAutoindex();
	std::string root = vs->getRoot();
	std::string index = vs->getIndex().front();
	std::vector<std::string> allowedMethods = vs->getAllowMethods();
	std::map<std::string, std::string> errorPages = vs->getErrorPages();
	string uri = parser->getUri();
	std::string file_requested = getFile(uri);
	if (!file_requested.empty())
	{
		uri = uri.substr(0, uri.find_last_of("/") + 1);
	}
	//std::cout << "uri: " << uri << "; file requested: " << file_requested << std::endl;
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
				dirent *tmp = readdir(d);
				while (tmp != NULL)
				{
					std::string elem = tmp->d_name;
					if (file_requested.compare(elem) == 0)
						use_CGI = true;
					tmp = readdir(d);
				}
				closedir(d);
			}
			else
			{
				setStatusCode(404);
				std::string err = vs->getErrorPages().find(numberToString(getStatusCode()))->second;
				setHeaders(*parser, vs->getMimeTypes(), err);
				response = toString();
				return response;
			}
		}
		allowedMethods = l->second.allow_methods;
		autoindex = l->second.autoindex;
		index = (l->second.index).front();
		errorPages = l->second.errorPages;
		root = l->second.root;
	}
	std::vector<std::string>::iterator it = allowedMethods.begin();
	while (it != allowedMethods.end())
	{
		if (*it == "GET")
			break;
		it++;
	}
	if (!allowedMethods.empty() && it == allowedMethods.end())
		setStatusCode(405);
	else
		setStatusCode(200);
	if (l != locations.end() && use_CGI && getStatusCode() == 200)
	{
		CGI cgi(parser, &(l->second), file_requested);
		setStatusCode(200);
		std::string buffer = cgi.CGI_Executer();
		if (cgi.getErr() == 1)
		{
			setStatusCode(500);
			std::string err = errorPages.find("500")->second;
			setHeaders(*parser, vs->getMimeTypes(), err);
			response = toString();
		}
		else
		{
			setHeaders_CGI(*parser, buffer);
			response = toString_CGI();
		}
		//std::cout << "response: " << response << std::endl;
		return response;
	}
	if (getStatusCode() != 200)
	{
		std::string err = errorPages.find(numberToString(getStatusCode()))->second;
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
		return response;
	}
	if (uri == "/" && file_requested.empty() && isValidFile((root + "/" + index).c_str()))
	{
		setStatusCode(200);
		setHeaders(*parser, vs->getMimeTypes(), root + "/" + index);
		response = toString();
	}
	else if (isValidFile((root + uri + file_requested).c_str()))
	{
		setStatusCode(200);
		setHeaders(*parser, vs->getMimeTypes(), root + uri + file_requested);
		response = toString();
	}
	else if (isValidDir((root + parser->path).c_str()) && autoindex && !isValidFile((root + index).c_str()))
	{
		std::string response_body = generateAutoindex(root, parser->path);
		//std::cout << response_body << std::endl;
		setStatusCode(200);
		std::string len = numberToString(response_body.size());
		std::string resp = "Content-Type: text/html\r\n";
		resp += "Content-Length: " + len + "\r\n\r\n";
		resp += response_body;
		setHeaders_CGI(*parser, resp);
		response = toString_CGI();
	}
	else if (l != locations.end() && !l->second.try_files.empty() && (l->second.try_files.front()).compare(file_requested) == 0)
	{
		std::vector<std::string> try_files = l->second.try_files;
		try_files.erase(try_files.begin());
		while (try_files.size() != 1)
		{
			if (isValidFile((root + "/" + try_files.front()).c_str()))
				break;
			try_files.erase(try_files.begin());
		}
		setStatusCode(301);
		setHeaders(*parser, vs->getMimeTypes(), try_files.front());
		response = toString();
	}
	else
	{
		setStatusCode(404);
		std::string err = errorPages.find("404")->second;
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	return response;
}
