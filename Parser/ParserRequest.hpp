#ifndef PARSERREQUEST_HPP
#define PARSERREQUEST_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>


void stampaCaratteriNonStampabili(const std::string& str);

using namespace std;

struct HTTPParser
{
	string method;
	string path;
	string protocol;

	map<string, string> headers;

	string body;
};

#endif
