/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/25 12:04:25 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"
#include <cstdlib>

GetResponse::GetResponse() : Response()
{
}

GetResponse::~GetResponse()
{
}

std::string GetResponse::answer(ParserRequest *parser)
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
		ifstream file("./web/index.html");
		std::string line;
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
				body += line;
			file.close();
		}
		else
			std::cout << "Unable to open file";
		response = "HTTP/1.1 200 OK\r\n";
		response += "Content-Type: text/html\r\n";
		std::string len = std::to_string(body.size());
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


