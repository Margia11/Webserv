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
	if(isValidFile(parser->path.c_str()))
	{
		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Content-Length: 12\r\n";
		response += "\r\n";
		response += "Hello World!";
	}
	else if (parser->path == "/")
	{
		std::string body("");
		std::cout << "---------------------PROVA---------------------" << std::endl;
		stampaCaratteriNonStampabili(vs->getRoot());
		std::cout << "---------------------PROVA---------------------" << std::endl;
		std::string root = vs->getRoot().substr(0, vs->getRoot().size() - 1);
		std::string index = vs->getIndex().substr(0, vs->getIndex().size() - 1);
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
		response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Content-Length: 0\r\n";
		response += "\r\n";
	}
	return response;
}


