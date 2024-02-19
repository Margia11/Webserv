/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:50 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/19 17:13:31 by andreamargi      ###   ########.fr       */
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
	std::cout << "Received POST request:\n";
	std::string uplodPath = vs->getUploadPath();
	std::cout << uplodPath << std::endl;
	std::string response;
	if (!isValidDir(uplodPath.c_str()))
	{
		if(!createDir(uplodPath.c_str()))
			setStatusCode(500);
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
				for (size_t i = 0; i < parts.size(); i++)
				{
					std::string filename;
					std::string fileContent;
					if (parts[i].find("filename") != std::string::npos)
					{
						filename = parts[i].substr(parts[i].find("filename=") + 10);
						filename = filename.substr(0, filename.find("\""));
						std::string fileContent = parts[i].substr(parts[i].find("\r\n\r\n") + 4);
					}
					else
					{
						filename = "post_data.txt";
						fileContent = "culo";
					}
					std::string filePath = uplodPath + "/" + filename;
					std::ofstream file(filePath.c_str());
					file << fileContent;
					file.close();
				}
			}
			else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
			{
				std::string body = parser->body;
				std::vector<std::string> pairs = vector_split(body, "&");
				std::string filePath = uplodPath + "/" + "post_data";
				std::ofstream file(filePath.c_str());
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
