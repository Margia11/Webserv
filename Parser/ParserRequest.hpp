/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 10:38:47 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/24 12:33:32 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERREQUEST_HPP
# define PARSERREQUEST_HPP

#include <string>
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
		void readRequest(int newsocket, char **buffer);
		void getRequestLine(string line);
		bool checkRequestLine(string line);
		void getHeaders(string line);
		ParserRequest();
		~ParserRequest();
};

void stampaCaratteriNonStampabili(std::string str);

#endif
