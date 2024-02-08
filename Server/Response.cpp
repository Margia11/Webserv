/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:06:34 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/26 11:55:39 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	statusCode = 200;
	protocol = "HTTP";
	version = "1.1";
	body = "Hello World!";

	httpStatus[200] = "OK";
    httpStatus[301] = "Moved Permanently";
    httpStatus[400] = "Bad Request";
    httpStatus[401] = "Unauthorized";
    httpStatus[403] = "Forbidden";
    httpStatus[404] = "Not Found";
    httpStatus[405] = "Method Not Allowed";
    httpStatus[413] = "Request Entity Too Large";
    httpStatus[415] = "Unsupported Media Type";
    httpStatus[500] = "Internal Server Error";
    httpStatus[501] = "Not Implemented";
    httpStatus[503] = "Service Unavailable";
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

std::map<int, std::string> Response::getHttpStatus()
{
	return httpStatus;
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

void Response::setDate()
{
	time_t now = time(0);
	char buff[20];
	strftime(buff, 20, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	headers["Date"] = buff;
}

void Response::setLastModified(const std::string &path)
{
	struct stat fileStat;
	stat(path.c_str(), &fileStat);
	char buff[20];
	strftime(buff, 20, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&fileStat.st_mtime));
	headers["Last-Modified"] = buff;
}

void Response::setContentType(const std::string &path, const std::map<std::string, std::string> &mimTypes)
{
	std::string str = mimTypes.find(getExtension(path))->second;
	if(str.empty())
		headers["Content-Type"] = "text/html";
	else
		headers["Content-Type"] = str;
}

void Response::setContentLength(int body_size)
{
	std::stringstream ss;
	ss << body_size;
	headers["Content-Length"] = ss.str();
}

void Response::setServer(std::string server)
{
	this->server = server;
	headers["Server"] = server;
}

void Response::setAcceptRanges(std::string acceptRanges)
{
	headers["Accept-Ranges"] = acceptRanges;
}

void Response::setConnection(std::string connection)
{
	headers["Connection"] = connection;
}

void Response::setHeaders(const ParserRequest &request, const std::map<string, string> &mimTypes, const string &path)
{
	if (statusCode == 200 || statusCode == 301)
        setLastModified(path);

    setProtocol(request.getProtocol());
    setContentType(path, mimTypes);
	setBody(getWholeFile(path));
    setContentLength(body.size());
    setDate();
    setServer("webserv 1.0");
    setAcceptRanges("bytes");
    setConnection("Keep-Alive");
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
	std::ifstream file;
	file.open(path.c_str());

	if(!file.is_open() || !isValidFile(path.c_str()))
		statusCode = 404;
	else if(str.empty()) //aggiungere controllo per vedere se è una get
		statusCode = 403;
	else
		statusCode = 200;
}

std::string Response::toString(void)
{
	std::stringstream ss;
	ss << this->protocol << " " << this->statusCode << " " << httpStatus[statusCode] << "\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		ss << it->first << ": " << it->second << "\r\n";
	ss << "\r\n";
	ss << body;
	return ss.str();
}