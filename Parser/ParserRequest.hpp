/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 10:38:47 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/27 11:01:59 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERREQUEST_HPP
# define PARSERREQUEST_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

class ParserRequest
{
	public:
		string method;
		string path;
		string protocol;
		map<string, string> headers;
		string body;
		void printparse();
		bool readRequest(std::string input);
		std::string getProtocol() const;
		bool parseRequestLine(string line);
		bool checkRequestLine(string line);
		bool parseHeaders(string line);
		bool checkBody();
		std::string getHost() const;
		ParserRequest();
		~ParserRequest();
		std::string getUri()const;
		std::string getQuery()const;
		std::string getRequestBody() const;
		void setRequestBody(std::string body);
};

void stampaCaratteriNonStampabili(std::string str);
#endif
