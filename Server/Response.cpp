/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:06:34 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/21 16:35:27 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
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

void Response::printHttpStatus()
{
	for (std::map<int, std::string>::iterator it = httpStatus.begin(); it != httpStatus.end(); ++it)
		std::cout << it->first << " => " << it->second << std::endl;
}

std::string Response::getContentType()
{
	return headers["Content-Type"];
}

void Response::setContentType(const std::string &path, const std::map<std::string, std::string> &mimTypes)
{
	std::string ext = getExtension(path);
	std::map<std::string, std::string>::const_iterator it = mimTypes.find(ext);
	if (it != mimTypes.end())
		headers["Content-Type"] = it->second;
	else
		headers["Content-Type"] = "text/html";
}

void Response::setContentLength(const std::string &path)
{
	int file_size;
	if (getContentType().compare("img/jpeg") == 0)
	{
		std::ifstream in_file(path.c_str(), std::ios::binary);
		in_file.seekg(0, std::ios::end);
		file_size = in_file.tellg();
	}
	else
		file_size = body.size();
	std::stringstream ss;
	ss << file_size;
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

void Response::setHeaders_CGI(const ParserRequest &request, const string &body)
{
	/* if (statusCode == 200 || statusCode == 301)
        setLastModified(path); */
	setProtocol(request.getProtocol());
	setBody(body);
	//setDate();
	setServer("webserv 1.0");
	setAcceptRanges("bytes");
	setConnection("Keep-Alive");
}


void Response::setHeaders(const ParserRequest &request, const std::map<string, string> &mimTypes, const string &path)
{

	/* if (statusCode == 200 || statusCode == 301)
        setLastModified(path); */
	setProtocol(request.getProtocol());
	setContentType(path, mimTypes);
	setBody(getWholeFile(path, getContentType()));
	setContentLength(path);
	if (getContentType().compare("img/jpeg") == 0)
		headers["Content-Disposition"] = "inline";
	setServer("webserv 1.0");
	setAcceptRanges("bytes");
	setConnection("Keep-Alive");
}

bool Response::isValidDir(const char* path)
{
	struct stat path_stat;
	if(stat(path, &path_stat) == 0) //controlla se il path esiste
	{
		//std::cout << "path exists" << std::endl;
		if(S_ISDIR(path_stat.st_mode)) //controlla se è una directory
			return true;
	}
	return false;
}

bool Response::createDir(const char* path)
{
	int risultato = std::system(("mkdir " + std::string(path)).c_str());
	if (risultato == 0)
		std::cout << "Dir created" << std::endl;
	else
		return 1;
	return 0;
}

std::string Response::getFile(std::string uri)
{
	std::string ext_file = getExtension(uri);
	if (ext_file.empty())
		return "";
	return uri.substr(uri.find_last_of("/") + 1, uri.size());
}

//estrae l'estensione del file
std::string Response::getExtension(const std::string &path)
{
	if(path.find_last_of(".") != std::string::npos)
		return path.substr(path.find_last_of("."));
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

std::string Response::toString_CGI(void)
{
	std::stringstream ss;
	ss << this->protocol << " " << this->statusCode << " " << httpStatus[statusCode] << "\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		ss << it->first << ": " << it->second << "\r\n";
	ss << body;
	return ss.str();
}
