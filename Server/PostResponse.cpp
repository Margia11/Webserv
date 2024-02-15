/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:50 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/15 17:46:18 by andreamargi      ###   ########.fr       */
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
	std::cout << uplodPath << std::endl;
	std::string response;
	if (!isValidDir(uplodPath.c_str()))
	{
		if(!createDir(uplodPath.c_str()))
			setStatusCode(500);
	}
	else
		setStatusCode(200);
	if (getStatusCode() != 200)
	{
		std::string err = vs->getErrorPages().find(numberToString(getStatusCode()))->second;
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	else
	{
		setHeaders(*parser, vs->getMimeTypes(), vs->getRoot() + "/" + vs->getIndex().front());
		response = toString();
	}
	/*{
		std::string path = uplodPath + "tmp.txt";
		std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			file << parser->body;
			file.close();
			setStatusCode(200);
			setHeaders(*parser, vs->getMimeTypes(), vs->getRoot() + "/" + vs->getIndex().front());
			response = toString();
		}
		else
		{
			std::cerr << "Error: cannot open file" << std::endl;
			setStatusCode(500);
			setHeaders(*parser, vs->getMimeTypes(), vs->getRoot() + "/" + vs->getIndex().front());
			response = toString();
			return response;
		}
	}
	else
	{
		std::cerr << "Error: invalid upload path" << std::endl;
		setStatusCode(500);
		setHeaders(*parser, vs->getMimeTypes(), vs->getRoot() + "/" + vs->getIndex().front());
		response = toString();
		return response;
	}*/
	return response;
}
