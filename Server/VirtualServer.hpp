/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:04:26 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/07 10:54:52 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "../Parser/ParserConf.hpp"
#include "../Parser/ParserRequest.hpp"
#include <poll.h>
#include <string>
#include <string.h>
#include <vector>

struct LocationInfo
{
	std::string root;
	std::vector<std::string> index;
	std::string autoindex;
	std::vector<std::string> allow_methods;
};

class VirtualServer
{
	private :
		std::vector<std::string> server_name;
		std::string root;
		std::vector<std::string> index;
		std::string uploadPath;
		std::map<std::string, std::string> errorPages;
		std::map<std::string, LocationInfo> locations;
		std::map<std::string, std::string> _mimeTypes;
		std::string client_max_body_size;
		//std::vector<std::string> allow_methods;
	public :
		VirtualServer(const ServerConfig& config);
		~VirtualServer();
		void printMimeType();
		const std::string& getUploadPath() const;
		const std::vector<std::string>& getServerName() const;
		const std::string& getRoot() const;
		const std::vector<std::string>& getIndex() const;
		const std::map<std::string, std::string>& getErrorPages() const;
		const std::map<std::string, LocationInfo>& getLocations() const;
		const std::string& getClientMaxBodySize() const;
		const std::map<std::string, std::string>& getMimeTypes() const;
		void _setMimeTypes();
		void printVServer();
};




#endif
