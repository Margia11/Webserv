/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 10:52:03 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/21 15:38:49 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void stringToASCII(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		std::cout << (int)str[i] << " ";
	}
	std::cout << std::endl;
}

// funzione che converte l'ip da stringa a byte order
u_int32_t string_to_byte_order(const char *ip_addr) {
	int octets[4];
	size_t i = 0;
	int j = 0;
	int h = 0;
	char tmp[3];

	if (ip_addr == NULL)
		return 0;

	while (i < strlen(ip_addr) + 1)
	{
		if (ip_addr[i] == '.' || i == (strlen(ip_addr)))
		{
			tmp[j] = 0;
			octets[h] = std::atoi(tmp);
			h++;
			j = 0;
			i++;
			continue;
		}
		tmp[j] = ip_addr[i];
		i++;
		j++;
	}
	for (i = 0; i < 4; i++) {
		if (octets[i] < 0 || octets[i] > 255)
			return 0;
	}
	return ((u_int32_t)octets[0] << 24) | ((u_int32_t)octets[1] << 16) | ((u_int32_t)octets[2] << 8) | ((u_int32_t)octets[3]);
}

//controllo se il path è valido e se è un file regolare
bool isValidFile(const char* path)
{
	struct stat path_stat;
	if(stat(path, &path_stat) == 0) //controlla se il path esiste
	{
		if(S_ISREG(path_stat.st_mode)) //controlla se è un file regolare (non una directory)
			return true;
	}
	return false;
}

//funzione che da host(stringa) ritorna il pair <host, port(int)>
std::pair<std::string, int>	toHostPort(std::string raw)
{
	size_t it = raw.find(":");
	std::string host = raw.substr(0, it);
	std::string str_port = raw.substr(it + 1, raw.size());
	std::stringstream ss(str_port);
	int port;
	ss >> port;
	std::pair<std::string, int> ret = make_pair(host, port);
	return ret;
}

std::string getWholeFile(const std::string &path, std::string contentType)
{
	std::string body("");
	std::ifstream file;
	if (contentType.compare("img/jpeg") == 0)
	{
		file.open(path.c_str(), std::ios::binary);
		// std::cout << "open img" << std::endl;
	}
	else
		file.open(path.c_str());
	std::string line;
	if (file.is_open())
	{
		std::stringstream  buffer;
        buffer << file.rdbuf();
		body += buffer.str();
		file.close();
	}
	else
		std::cout << "Unable to open file " << path << std::endl;
	return body;
}

std::string numberToString(int n)
{
	std::string s;
	std::stringstream out;
	out << n;
	s = out.str();
	return s;
}
