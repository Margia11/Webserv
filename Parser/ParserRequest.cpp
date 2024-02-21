#include "ParserRequest.hpp"

std::string ParserRequest::getUri() const
{
	size_t found = this->path.find("?");
	if (found != std::string::npos)
		return this->path.substr(0, found);
	return this->path;
}

std::string ParserRequest::getQuery() const
{
	size_t found = this->path.find("?");
	if (found != std::string::npos)
		return this->path.substr(found + 1);
	return "";
}

bool ParserRequest::checkRequestLine(string line)
{
	size_t	first_space = line.find(' ');
	if (first_space == string::npos || first_space == 0 || first_space == line.size())
		return false;
	size_t	second_space = line.find(' ', first_space + 1);
	if (second_space == string::npos || second_space == 0 || second_space == line.size())
		return false;
	size_t	third_space = line.find(' ', second_space + 1);
	if (third_space != string::npos)
		return false;
	istringstream issRequest(line);
	string reqMethod;
	string reqPath;
	string reqProtocol;
	issRequest >> reqMethod >> reqPath >> reqProtocol;
	this->method = reqMethod;
	this->path = reqPath;
	this->protocol = reqProtocol;
	return true;
}

void	ParserRequest::getHeaders(string line)
{
	istringstream issHeader(line);
	std::string headerName;
	std::string headerValue;
	issHeader >> headerName >> headerValue;
	std::string buf;
	while (issHeader >> buf)
	{
		headerValue += " " + buf;
	}
	headerName = headerName.substr(0, headerName.size() - 1);
	this->headers.insert(std::make_pair(headerName, headerValue));
}

std::string ParserRequest::getProtocol() const
{
	return this->protocol;
}

void ParserRequest::getRequestLine(string line)
{
	if (line.empty())
		return ;
	if (!checkRequestLine(line))
		return ;
}

void stampaCaratteriNonStampabili(std::string str)
{
	const char *str2 = str.c_str();
	int i = 0;
	while (str2[i])
	{
		char c = str2[i];
		if (isprint(c))
			std::cout << c;
		else
			std::cout << "\\x" << std::setw(2) << std::setfill('0') << std::hex << (int)(unsigned char)c;
		i++;
	}
	std::cout << std::endl;
}

bool ParserRequest::readRequest(std::string input)
{
	size_t reqLinePos = input.find("\r\n");
	if (reqLinePos == string::npos)
		return false;
	std::string reqLine = input.substr(0, reqLinePos);
	getRequestLine(reqLine);
	size_t headerPos = input.find("\r\n", reqLinePos + 2);
	if (headerPos == string::npos)
		return false;
	std::string headers = input.substr(reqLinePos + 2, headerPos - reqLinePos - 2);
	while(!headers.empty())
	{
		size_t oldHeaderPos = headerPos;
		getHeaders(headers);
		headerPos = input.find("\r\n", oldHeaderPos + 2);
		headers = input.substr(oldHeaderPos + 2, headerPos - oldHeaderPos - 2);
	}
/* 	map<string, string>::iterator it = this->headers.find("Content-Length");
	int len = 0;
	if (it != this->headers.end())
		len = atoi(it->second.c_str()); */
	this->body = input.substr(headerPos + 2, input.size());
	return true;
}

void ParserRequest::printparse()
{
	cout << "Method: " << this->method << endl;
	cout << "Path: " << this->path << endl;
	cout << "Protocol: " << this->protocol << endl;
	cout << "Headers: " << endl;
	for (map<string, string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}
	cout << "Body: " << this->body << endl;
}
ParserRequest::ParserRequest()
{
	this->method = "";
	this->path = "";
	this->protocol = "";
	this->headers = map<string, string>();
	this->body = "";
}

ParserRequest::~ParserRequest()
{
}

std::string ParserRequest::getHost()
{
	map<string, string>::iterator it = this->headers.find("Host");
	if (it != this->headers.end())
		return it->second;
	return "";
}
