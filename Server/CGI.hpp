/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:22:23 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/19 16:49:05 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "MainServer.hpp"

class CGI
{
	private :
		std::map<std::string, std::string> CGI_env;
		char **env_execve;
		std::string request_body;
		std::string response_body;
		char **arg_execve;
	public :
		std::vector<std::string> CGI_path;
		CGI(ParserRequest *parser, LocationInfo *location);
		~CGI();
		std::string CGI_Executer();
		std::string getCmd();
};



#endif
