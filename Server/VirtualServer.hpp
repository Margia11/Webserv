/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:04:26 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/22 15:40:49 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <poll.h>
#include <string>
#include <string.h>
#include <vector>
#include "utils.hpp"
#include "../Parser/ParserConf.hpp"
#include "../Parser/ParserRequest.hpp"


struct LocationInfo
{
	std::string root;
	std::vector<std::string> index;
	std::string uploadPath;
	std::map<std::string, std::string> errorPages;
	long unsigned int client_max_body_size;
	bool autoindex;
	std::string cgi_path;
	std::vector<std::string> allow_methods;
	std::vector<std::string> try_files;
	std::string path;
};

class VirtualServer
{
	private :
		std::vector<std::string> server_name;
		std::string root;
		std::vector<std::string> index;
		std::string uploadPath;
		bool autoindex;
		std::map<std::string, std::string> errorPages;
		std::map<std::string, LocationInfo> locations;
		std::map<std::string, std::string> _mimeTypes;
		long unsigned int client_max_body_size;
		std::vector<std::string> allow_methods;
	public :
		VirtualServer(const ServerConfig& config);
		~VirtualServer();
		void printMimeType();
		const std::string& getUploadPath() const;
		const std::vector<std::string>& getServerName() const;
		const std::vector<std::string>& getAllowMethods() const;
		const std::string& getRoot() const;
		const std::vector<std::string>& getIndex() const;
		const std::map<std::string, std::string>& getErrorPages() const;
		const std::map<std::string, LocationInfo>& getLocations() const;
		long unsigned int getClientMaxBodySize() const;
		const std::map<std::string, std::string>& getMimeTypes() const;
		bool getAutoindex() const;
		//LocationInfo *matchLocation(std::map<std::string, LocationInfo> locations, std::string uri);
		void _setMimeTypes();
		void printVServer();
		void printLocations(VirtualServer *vs);
		std::string const &getValidIndex() const;
};




#endif
