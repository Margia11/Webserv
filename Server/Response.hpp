/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 13:52:21 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/25 11:45:21 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include "../Parser/ParserRequest.hpp"
#include <sstream>
#include <fstream>

class Response
{
	private:
		std::string allow;
		std::string locations;
		std::string retryafter;
		std::string server;
		int statusCode;
		std::string protocol;
		std::string version;
		std::string body;
		std::map<std::string, std::string> headers;
		std::map<int, std::string> httpErrors;
	public:
		Response();
		virtual ~Response();
		int getStatusCode();
		std::string getProtocol();
		std::string getVersion();
		std::string getBody();
		std::map<std::string, std::string> getHeaders();
		std::map<int, std::string> getHttpErrors();
		void setStatusCode(int statusCode);
		void setStatusCode(const std::string& path, std::map<std::string, std::string> &mimTypes);
		void setProtocol(std::string protocol);
		void setVersion(std::string version);
		void setBody(std::string body);
		void setHeaders(std::map<std::string, std::string> headers);
		void setHttpErrors(std::map<int, std::string> httpErrors);
		bool isValidFile(const char* path);
		std::string getExtension(const std::string &path);
		virtual std::string answer(ParserRequest *parser) = 0;
};


#endif
