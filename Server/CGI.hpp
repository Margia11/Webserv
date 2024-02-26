/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:22:23 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/26 15:48:50 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/wait.h>
#include "MainServer.hpp"

class CGI
{
	private :
		std::map<std::string, std::string> CGI_env;
		char **env_execve;
		std::string request_body;
		int err;
		std::string response_body;
		char **arg_execve;
	public :
		std::vector<std::string> CGI_path;
		CGI(ParserRequest *parser, LocationInfo *location, std::string script);
		~CGI();
		std::string CGI_Executer();
		std::string getCmd();
		int getErr();
};



#endif
