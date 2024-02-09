/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/03 10:25:06 by andreamargi      ###   ########.fr       */
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
	std::string root = vs->getRoot().substr(0, vs->getRoot().size() - 1);
	std::string index = vs->getIndex().front().substr(0, vs->getIndex().front().size() - 1);
	int x = isValidFile((root + "/" + index).c_str());
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
		err = err.substr(0, err.size() - 1);
		setStatusCode(404);
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	return response;
}

/*
std::string GetResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::string root = vs->getRoot().substr(0, vs->getRoot().size() - 1);
	std::string index = vs->getIndex().front();
	std::string response;
	//check parser path = "/" && root + "/" + index -> else error
	//else if parser path != "/" -> parser path
	if(isValidFile((root + "/" + parser->path).c_str()))
	{
		std::string body("");
		ifstream file((root + "/" + parser->path).c_str());
		std::string line;
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
				body += line;
			file.close();
		}
		else
			std::cout << " to open file";
		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: text/html\r\n";
		std::stringstream ss;
		ss << body.size();
		std::string len = ss.str();
		response += "Content-Length: " + len + "\r\n";
		response += "\r\n";
		response += body;
	}
	else if (parser->path == "/" && isValidFile((root + "/" + index).c_str()))
	{
		std::string body("");
		std::string path = root + "/"+ index;
		std::cout << "path: " << path << std::endl;
		ifstream file(path.c_str());
		std::string line;
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
				body += line;
			file.close();
		}
		else
			std::cout << " to open file";
		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: text/html\r\n";
		std::stringstream ss;
		ss << body.size();
		std::string len = ss.str();
		response += "Content-Length: " + len + "\r\n";
		response += "\r\n";
		response += body;
	}
	else
	{
		std::string body("");
		std::string error_page = vs->getErrorPages().find("404")->second;
		error_page = error_page.substr(0, error_page.size() - 1);
		std::cout << "error: " << error_page << std::endl;
		ifstream file(error_page.c_str());
		std::string line;
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
				body += line;
			file.close();
		}
		else
			std::cout << " to open file";
		response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Content-Type: text/html\r\n";
		std::stringstream ss;
		ss << body.size();
		std::string len = ss.str();
		response += "Content-Length: " + len + "\r\n";
		response += "\r\n";
		response += body;
	}
	return response;
}
*/
