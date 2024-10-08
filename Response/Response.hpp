/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 13:52:21 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/07 11:17:17 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include "../Parser/ParserRequest.hpp"
#include "../Server/VirtualServer.hpp"
#include "../Server/utils.hpp"

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
		std::map<int, std::string> httpStatus;
	public:
		Response();
		virtual ~Response();
		int getStatusCode();
		std::string getProtocol();
		std::string getVersion();
		std::string getBody();
		std::map<std::string, std::string> getHeaders();
		std::map<int, std::string> getHttpStatus();
		void setLastModified(const std::string &path);
		void setStatusCode(int statusCode);
		void setStatusCode(const std::string& path, std::map<std::string, std::string> &mimTypes);
		void setProtocol(std::string protocol);
		void setVersion(std::string version);
		void setBody(std::string body);
		void setContentLength(const std::string &path);
		void setDate();
		void setServer(std::string server);
		void setAcceptRanges(std::string path);
		void setConnection(std::string connection);
		void setContentType(const std::string &path, const std::map<std::string, std::string> &mimTypes);
		void setHeaders(const ParserRequest &request, const std::map<string, string> &mimTypes, const string &path);
		void setHeaders_CGI(const ParserRequest &request, const string &body);
		void printHttpStatus();
		bool isValidDir(const char* path);
		bool createDir(const std::string& path);
		std::string getExtension(const std::string &path);
		virtual std::string answer(ParserRequest *parser, VirtualServer *vs) = 0;
		std::string toString(void);
		std::string toString_CGI(void);
		std::string getFile(std::string uri);
		std::string	getContentType();
		void setHost(std::string host);
		void setLocation(std::string path);
};

#endif
