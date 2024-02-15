/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:50 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/14 10:22:59 by andreamargi      ###   ########.fr       */
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
	std::string uplodPath = vs->getUploadPath();

	if (isValidDir(uplodPath.c_str()))
	{
		std::string path = uplodPath + "tmp.txt";
		std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file << parser->body;
			file.close();
		}
		else
		{
			std::cerr << "Error: cannot open file" << std::endl;
			std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
			return response;
		}
	}
	else
	{
		std::cerr << "Error: invalid upload path" << std::endl;
		std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
		return response;
	}

	std::string response = "HTTP/1.1 200 OK\r\n";
	return response;
}
