/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:04:26 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/23 15:52:14 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "SimpleServer.hpp"
#include "../Parser/ParserConf.hpp"
#include <poll.h>
#include <string>
#include <vector>

struct LocationInfo
{
	std::string root;
	std::string index;
	std::string autoindex;
	std::vector<std::string> allow_methods;
};

class VirtualServer : public Server
{
	private :
		std::string server_name;
		int port;
		std::string host;
		std::string root;
		std::string index;
		std::map<std::string, std::string> errorPages;
		std::map<std::string, LocationInfo> locations;
		std::string client_max_body_size;
		//std::vector<std::string> allow_methods;
		char buffer[30000];
		int newsocket;
		static const int MAX_EVENTS = 10;
		struct pollfd fds[MAX_EVENTS];
		void Accepter();
		void Handler();
		void Responder();
	public :
		VirtualServer(ServerConfig config);
		virtual ~VirtualServer();
		const std::string& getServerName() const;
		const std::string& getRoot() const;
		const std::string& getIndex() const;
		const std::map<std::string, std::string>& getErrorPages() const;
		const std::map<std::string, LocationInfo>& getLocations() const;
		const std::string& getClientMaxBodySize() const;
		const std::string& getHost() const;
		int getPort() const;
		void launch();
};




#endif
