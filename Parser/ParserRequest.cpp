#include "ParserRequest.hpp"

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
	this->headers.insert(std::make_pair(headerName, headerValue));
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

void ParserRequest::readRequest(int newsocket)
{
	char *buffer = new char[32000];
	int byte_read;
	byte_read = read(newsocket, buffer, 32000);
	std::cout << "buffer: " << buffer << std::endl;
	if (byte_read < 0)
	{
		perror("read");
		exit(1);
	}
	else if (byte_read == 0)
	{
		cout << "Client closed connection" << endl;
		return ;
	}
	else
	{
		std::string str(buffer, byte_read);
		size_t reqLinePos = str.find("\r\n");
		std::string reqLine = str.substr(0, reqLinePos);
		getRequestLine(reqLine);
		size_t headerPos = str.find("\r\n", reqLinePos + 2);
		std::string headers = str.substr(reqLinePos + 2, headerPos - reqLinePos - 2);
		while(!headers.empty())
		{
			size_t oldHeaderPos = headerPos;
			getHeaders(headers);
			headerPos = str.find("\r\n", oldHeaderPos + 2);
			headers = str.substr(oldHeaderPos + 2, headerPos - oldHeaderPos - 2);
		}
		map<string, string>::iterator it = this->headers.find("Content-Length");
		int len = 0;
		if (it != this->headers.end())
		{
			len = atoi(it->second.c_str());
			cout << "Content-Length: " << len << endl;
		}
		else
			cout << "Content-Length: " << 0 << endl;
		int readFromBody = 32000 - headerPos;
		this->body = str.substr(headerPos + 2, str.size());
		while (readFromBody < len)
		{
			byte_read = read(newsocket, buffer, 32000);
			if (byte_read < 0)
			{
				perror("read");
				exit(1);
			}
			else if (byte_read == 0)
			{
				cout << "Client closed connection" << endl;
				return ;
			}
			else
			{
				this->body += std::string(buffer, byte_read);
				readFromBody += byte_read;
				cout << "readFromBody: " << readFromBody << endl;
			}
		}
	}
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
