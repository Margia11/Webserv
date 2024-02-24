/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:50 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/24 11:08:09 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostResponse.hpp"
#include <sstream>

PostResponse::PostResponse()
{
}

PostResponse::~PostResponse()
{
}

std::vector<std::string> vector_split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	result.push_back(str);
	return result;
}

std::string PostResponse::answer(ParserRequest *parser, VirtualServer *vs)
{
	std::string response;
	std::string uploadPath = vs->getUploadPath();
	string clientMaxBodySize = vs->getClientMaxBodySize();
	std::vector<std::string> allowedMethods = vs->getAllowMethods();
	std::map<std::string, std::string> errorPages = vs->getErrorPages();
	string root = vs->getRoot();
	string uri = parser->getUri();
	if (uri.empty())
		uri = "/";
	std::vector<std::string> index = vs->getIndex();
	std::string file_requested = getFile(uri);
	std::cout << "file_requested: " << file_requested << std::endl;
	if (!file_requested.empty())
	{
		uri = uri.substr(0, uri.find_last_of("/") + 1);
	}
	std::map<std::string, LocationInfo> locations = vs->getLocations();
	std::map<std::string, LocationInfo>::iterator l = locations.find(uri);
	bool use_CGI = false;
	if (l != locations.end())
	{
		if (!l->second.cgi_path.empty())
		{
			DIR *d = opendir((root + l->second.cgi_path).c_str());
			if (d)
			{
				dirent *tmp = readdir(d);
				while (tmp != NULL)
				{
					std::string elem = tmp->d_name;
					if (file_requested.compare(elem) == 0)
						use_CGI = true;
					tmp = readdir(d);
				}
				closedir(d);
			}
			else
			{
				setStatusCode(404);
				std::string err = vs->getErrorPages().find(numberToString(getStatusCode()))->second;
				setHeaders(*parser, vs->getMimeTypes(), err);
				response = toString();
				return response;
			}
		}
		//uploadPath = l->second.uploadPath;
		allowedMethods = l->second.allow_methods;
		//errorPages = l->second.errorPages;
		//root = l->second.root;
		//clientMaxBodySize = l->second.clientMaxBodySize;
	}
	std::vector<std::string>::iterator it = allowedMethods.begin();
	while (it != allowedMethods.end())
	{
		if (*it == "POST")
			break;
		it++;
	}
	if (!allowedMethods.empty() && it == allowedMethods.end())
	setStatusCode(405);
	// else if (request.getBody().size() > convertToBytes(clientMaxBodySize))
	//	setStatusCode(413);
	if (!isValidDir(uploadPath.c_str()))
	{
		if(!createDir(uploadPath.c_str()))
			setStatusCode(500);
	}
	if (l != locations.end() && use_CGI)
	{
		CGI cgi(parser, &(l->second), file_requested);
		setStatusCode(200);
		std::string buffer = cgi.CGI_Executer();
		setHeaders_CGI(*parser, buffer);
		response = toString_CGI();
		//std::cout << "response: " << response << std::endl;
		return response;
	}
	else
		setStatusCode(200);
	if (getStatusCode() != 200)
	{
		std::string err = vs->getErrorPages().find(numberToString(getStatusCode()))->second;
		setHeaders(*parser, vs->getMimeTypes(), err);
		response = toString();
	}
	else
	{
		//check if the content type is multipart/form-data or application/x-www-form-urlencoded
		if (parser->headers.find("Content-Type") != parser->headers.end())
		{
			std::string contentType = parser->headers.find("Content-Type")->second;
			if (contentType.find("multipart/form-data") != std::string::npos)
			{
				std::string boundary = contentType.substr(contentType.find("boundary=") + 9);
				std::string body = parser->body;
				std::vector<std::string> parts = vector_split(body, "--" + boundary);
				std::ofstream file((uploadPath + "/post_data.txt").c_str());
				std::string fileContent;
				for (size_t i = 0; i < parts.size(); i++)
				{
					if (parts[i].find("filename") != std::string::npos)
					{
						std::string filename = parts[i].substr(parts[i].find("filename=") + 10);
						filename = filename.substr(0, filename.find("\""));
						std::string uploadedFileContent = parts[i].substr(parts[i].find("\r\n\r\n") + 4);
						std::string filePath = uploadPath + "/" + filename;
						std::ofstream uploadedFile(filePath.c_str(), std::ios::in | std::ios::binary);
						uploadedFile << uploadedFileContent;
						uploadedFile.close();
						fileContent += "file: " + filename;
					}
					else if (parts[i].find("name=") != std::string::npos)
					{

						std::string key = parts[i].substr(parts[i].find("name=") + 6);
						key = key.substr(0, key.find("\""));
						std::string value = parts[i].substr(0, parts[i].find_last_of("\r\n") - 1);
						value = value.substr(value.find_last_of("\r\n") + 1, value.size());
						fileContent += key + ": " + value + "\r\n";
					}
				}
				file << fileContent;
				file.close();
			}
			else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
			{
				std::string body = parser->body;
				std::vector<std::string> pairs = vector_split(body, "&");
				std::string filePath = uploadPath + "/" + "post_data";
				std::ofstream file(filePath.c_str(), std::ios::in | std::ios::binary);
				for (size_t i = 0; i < pairs.size(); i++)
				{
					std::string key = pairs[i].substr(0, pairs[i].find("="));
					std::string value = pairs[i].substr(pairs[i].find("=") + 1);
					file << value;
					if (i == pairs.size() - 1)
						file << std::endl;
					else
						file << ",";
				}
				file.close();
			}
		}
		setHeaders(*parser, vs->getMimeTypes(), "./web/html/success_upload.html");
		response = toString();
	}
	return response;
}

