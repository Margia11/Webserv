/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:06:34 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/25 11:42:31 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	statusCode = 200;
	protocol = "HTTP";
	version = "1.1";
	body = "Hello World!";
	httpErrors[100] = "Continue";
	httpErrors[200] = "OK";
	httpErrors[400] = "Bad Request";
	httpErrors[403] = "Forbidden";
	httpErrors[404] = "Not Found";
	httpErrors[500] = "Internal Server Error";
	httpErrors[503] = "Service Unavailable";
}

Response::~Response()
{
}

int Response::getStatusCode()
{
	return statusCode;
}

std::string Response::getProtocol()
{
	return protocol;
}

std::string Response::getVersion()
{
	return version;
}

std::string Response::getBody()
{
	return body;
}

std::map<std::string, std::string> Response::getHeaders()
{
	return headers;
}

std::map<int, std::string> Response::getHttpErrors()
{
	return httpErrors;
}

void Response::setStatusCode(int statusCode)
{
	this->statusCode = statusCode;
}

void Response::setProtocol(std::string protocol)
{
	this->protocol = protocol;
}

void Response::setVersion(std::string version)
{
	this->version = version;
}

void Response::setBody(std::string body)
{
	this->body = body;
}

//controllo se il path è valido e se è un file regolare
bool Response::isValidFile(const char* path)
{
	struct stat path_stat;
	if(stat(path, &path_stat) == 0) //controlla se il path esiste
	{
		if(S_ISREG(path_stat.st_mode)) //controlla se è un file regolare (non una directory)
			return true;
	}
	return false;
}

//estrae l'estensione del file
std::string Response::getExtension(const std::string &path)
{
	if(path.find_last_of(".") != std::string::npos)
		return path.substr(path.find_last_of(".") + 1);
	return "";
}

void Response::setStatusCode(const std::string& path, std::map<std::string, std::string> &mimTypes)
{
	std::string str = mimTypes[getExtension(path)];
	std::ifstream file(path);

	if(!file.is_open() || !isValidFile(path.c_str()))
		statusCode = 404;
	else if(str.empty()) //aggiungere controllo per vedere se è una get
		statusCode = 403;
	else
		statusCode = 200;
}
