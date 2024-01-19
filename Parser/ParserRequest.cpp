#include "ParserRequest.hpp"

#include <string>
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <map>

static bool	checkRequestLine(string line, HTTPParser *parse)
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
	parse->method = reqMethod;
	parse->path = reqPath;
	parse->protocol = reqProtocol;
	return true;
}

static void	getHeaders(string line, HTTPParser* parse)
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
	parse->headers.insert(std::make_pair(headerName, headerValue));
	cout << "$" << headerName << "$" << endl;
	cout << "$" << headerValue << "$"  << endl;
}

static void getRequestLine(string line, HTTPParser* parse)
{
	if (line.empty())
		return ;
	if (!checkRequestLine(line, parse))
		return ;
	cout << "$" << parse->method << "$" << endl;
	cout << "$" << parse->path << "$"  << endl;
	cout << "$" << parse->protocol << "$" << endl;
}

#include <iostream>
#include <iomanip>

void stampaCaratteriNonStampabili(const std::string& str) {
    for (char c : str) {
        if (isprint(c)) {
            std::cout << c;
        } else {
            std::cout << "\\x" << std::setw(2) << std::setfill('0') << std::hex << (int)(unsigned char)c;
        }
    }
    std::cout << std::endl;
}

int	main(void)
{
	ifstream	file;
	HTTPParser	parse;

	file.open("request.http", std::ios::in);
	while (file)
	{
		string line;
		bool	isRequestLine = true;
		bool	isHeader = false;
		bool	isBody = false;
		while (getline(file, line))
		{
			if (isRequestLine)
				getRequestLine(line, &parse);
			if (line.compare("\r") == 0)
			{
				isHeader = false;
				isBody = true;
			}
			if (isHeader)
				getHeaders(line, &parse);
			else if (isBody && line.compare("\r") != 0)
				parse.body += line;
			if (isRequestLine)
			{
				isRequestLine = false;
				isHeader = true;
			}
		}
		if (!file.eof())
			return (1);
		file.close();
	}
}
