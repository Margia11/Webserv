/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:02 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/20 10:57:45 by andreamargi      ###   ########.fr       */
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
	vs->printLocations(vs);
	std::string response;
	std::string root = vs->getRoot();
	std::string index = vs->getIndex().front();
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
		setStatusCode(404);
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	return response;
}
