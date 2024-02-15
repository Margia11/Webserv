/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 10:52:03 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/15 17:41:00 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

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

std::string getWholeFile(const std::string &path)
{
	std::string body("");
	std::ifstream file(path.c_str());
	std::string line;
	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
			body += line;
		file.close();
	}
	else
		std::cout << "Unable to open file";
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
