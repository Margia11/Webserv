/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:26:45 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/20 15:46:03 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(ParserRequest *parser, LocationInfo *location)
{
	CGI_env["SERVER_SOFTWARE"] = "webserv/1.0";
	CGI_env["SERVER_NAME"] = toHostPort(parser->getHost()).first;
	CGI_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	CGI_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	CGI_env["SERVER_PORT"] = toHostPort(parser->getHost()).second;
	CGI_env["REQUEST_METHOD"] = parser->method;
	CGI_env["REQUEST_URI"] = parser->getUri();
	CGI_env["PATH_INFO"] = location->root + parser->getUri();
	CGI_env["SCRIPT_NAME"] = parser->getUri();
	CGI_env["QUERY_STRING"] = parser->getQuery();
	CGI_env["CONTENT_LENGTH"] = parser->headers.find("Content-Length")->second;
	CGI_env["CONTENT_TYPE"] = parser->headers.find("Content-Type")->second;
	CGI_env["HTTP_ACCEPT"] = parser->headers.find("Accept")->second;
	CGI_env["HTTP_ACCEPT_LANGUAGE"] = parser->headers.find("Accept-Language")->second;
	CGI_env["HTTP_ACCEPT_ENCODING"] = parser->headers.find("Accept-Encoding")->second;
	CGI_env["HTTP_CONNECTION"] = parser->headers.find("Connection")->second;
	CGI_env["HTTP_HOST"] = toHostPort(parser->getHost()).first;
	CGI_env["HTTP_USER_AGENT"] = parser->headers.find("User-Agent")->second;
	CGI_env["HTTP_REFERER"] = parser->headers.find("Referer")->second;
	CGI_env["HTTP_COOKIE"] = parser->headers.find("Cookie")->second;

	CGI_path.push_back("script.cgi");
	CGI_path.push_back("get.py");
	env_execve = new char*[CGI_env.size() + 1];
	std::map<std::string, std::string>::iterator it = CGI_env.begin();
	for (int i = 0; it != CGI_env.end(); it++, i++)
	{
		env_execve[i] = new char[it->first.size() + it->second.size() + 2];
		strcpy(env_execve[i], (it->first + "=" + it->second).c_str());
	}
	env_execve[CGI_env.size()] = NULL;
	response_body = "";
	request_body = parser->body;
	arg_execve = new char*[2];
	arg_execve[1] = NULL;
}

CGI::~CGI()
{
	for (size_t i = 0; i < CGI_env.size(); i++)
		delete[] env_execve[i];
	delete[] env_execve;
	delete[] arg_execve;
}

//controlla quale script eseguire in base alla richiesta
std::string CGI::getCmd()
{
	string path_ = CGI_env["PATH_INFO"];
	string scriptName_ = path_.substr(path_.find_last_of('/') + 1);
	string extension_ = ".py";
	string cmd_ = "./web/cgi-bin/";
	std::vector<std::string>::iterator it = find(CGI_path.begin(), CGI_path.end(), scriptName_);
	if (it != CGI_path.end())
		return  cmd_ += *it;
	else
	{
		std::size_t pos = path_.find_last_of('.');
		if (pos != std::string::npos)
			extension_ = path_.substr(pos);
		for (std::vector<std::string>::iterator it = CGI_path.begin(); it != CGI_path.end(); ++it)
		{
			if (it->size() >= extension_.length() && it->substr(it->size() - extension_.length()) == extension_)
				return cmd_ += *it;
		}
	}
	return cmd_ += "helloCGI.py" ;
}

std::string CGI::CGI_Executer()
{
	arg_execve[0] = new char[getCmd().size() + 1];
	strcpy(arg_execve[0], getCmd().c_str());
	int fdReq[2];
	int fdResp[2];

	if (pipe(fdReq) == -1 || pipe(fdResp) == -1)
	{
		std::cerr << "Error in pipe" << std::endl;
		exit(1);
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error in fork" << std::endl;
		exit(1);
	}
	if(pid == 0)
	{
		dup2(fdReq[0], STDIN_FILENO);
		dup2(fdResp[1], STDOUT_FILENO);
		close(fdReq[1]);
		close(fdResp[0]);
		execve(arg_execve[0], arg_execve, env_execve);
	}
	else
	{
		close(fdReq[0]);
		close(fdResp[1]);
		write(fdReq[1], request_body.c_str(), request_body.size());
		close(fdReq[1]);
		int status;
		waitpid(pid, &status, 0);
		char buffer[1024];
		int n;
		bzero(buffer, 1024);
		while ((n = read(fdResp[0], buffer, 1024)) > 0)
		{
			buffer[n] = '\0';
			response_body += buffer;
			bzero(buffer, 1024);
		}
		close(fdResp[0]);
	}
	return response_body;
}
