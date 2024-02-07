/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:50 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/26 10:33:57 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostResponse.hpp"
#include <sstream>

PostResponse::PostResponse()
{
}

PostResponse::~PostResponse()
{
}

std::string PostResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::cout << "Received POST request:\n";
	std::cout << "root: " << vs->getRoot() << ", index: " << vs->getIndex() << std::endl;
	std::string postData = parser->body;
	std::string path = parser->path;
	if (path == "/")
		path = "/index.html";
	std::ifstream file(path.c_str());
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	std::stringstream ss;
	ss << postData.size();
	std::string len = ss.str();
	response += "Content-Length: " + len + "\r\n";
	response += "\r\n";
	response += postData;
	return response;
}
