/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:32:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/03 10:16:05 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

// funzione che converte l'ip da stringa a byte order
static u_int32_t string_to_byte_order(const char *ip_addr) {
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


VirtualServer::VirtualServer(ServerConfig config) : Server(AF_INET, SOCK_STREAM, 0, config.port, string_to_byte_order(config.host.c_str()), 10)
{
	this->server_name = config.server_name;
	this->port = config.port;
	this->host = config.host;
	this->root = config.root;
	this->index = config.index;
	this->errorPages = config.errorPages;
	this->client_max_body_size = config.client_max_body_size;
	this->buffer = (char *) malloc (32000);
	for (std::map<std::string, LocationConfig>::const_iterator it = config.locations.begin(); it != config.locations.end(); it++)
	{
		LocationInfo locationInfo;
		locationInfo.root = it->second.root;
		locationInfo.index = it->second.index;
		locationInfo.autoindex = it->second.autoindex;
		locationInfo.allow_methods = it->second.allow_methods;
		this->locations[it->first] = locationInfo;
	}
	this->parser = new ParserRequest();
	std::cout << "Socket "<< getSocket() << " created" << std::endl;
	//launch();
}

VirtualServer::~VirtualServer()
{
}

std::string const &VirtualServer::getClientMaxBodySize() const
{
	return (this->client_max_body_size);
}

std::string const &VirtualServer::getServerName() const
{
	return (this->server_name);
}

std::string const &VirtualServer::getRoot() const
{
	return (this->root);
}

std::string const &VirtualServer::getIndex() const
{
	return (this->index);
}

std::map <std::string, std::string> const &VirtualServer::getErrorPages() const
{
	return (this->errorPages);
}

std::map <std::string, LocationInfo> const &VirtualServer::getLocations() const
{
	return (this->locations);
}

int VirtualServer::getPort() const
{
	return (this->port);
}

std::string const &VirtualServer::getHost() const
{
	return (this->host);
}

void VirtualServer::Accepter()
{
	struct sockaddr_in address = getSocket()->getAddress();
	int addrlen = sizeof(address);
	newsocket = accept(getSocket()->getSocket(), (struct sockaddr *)&address,
			(socklen_t*)&addrlen);
	//read(newsocket, buffer, 30000);
	this->parser->readRequest(newsocket);
	//cerco un posto libero nella lista dei file descriptor
	//e lo preparo per la scrittura
	for (int i = 1; i < MAX_EVENTS; i++)
	{
		if (fds[i].fd == -1)
		{
			fds[i].fd = newsocket;
			fds[i].events = POLLOUT;
			break;
		}
	}
}

void VirtualServer::Handler()
{
}

// void VirtualServer::deleteRequest(ParserRequest *parser)
// {
// 	std::string path = parser->path;
// 	if (path.empty() || path == "/")
// 	{
// 		send(newsocket, "HTTP/1.1 403 Forbidden\r\n\r\n", 26, 0);
// 		return;
// 	}
// 	path = this->root + path;
// 	if (remove(path.c_str()) != 0)
// 		send(newsocket, "HTTP/1.1 403 Forbidden\r\n\r\n", 26, 0);
// 	else
// 		send(newsocket, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);
// }

void VirtualServer::Responder()
{
	GetResponse response;
	PostResponse postResponse;
	std::string answer;

	std::cout << "Received request:\n";
	std::cout << "Method: " << parser->method << "\n";
	std::cout << "Path: " << parser->path << "\n";
	std::cout << "Content-Type: " << parser->headers["Content-Type"] << "\n";
	std::cout << "Body:\n" << parser->body << "\n";
	if (parser->method == "GET")
		answer = response.answer(parser);
	else if (parser->method == "POST")
		answer = postResponse.answer(parser);
	else if (parser->method == "DELETE")
		std::cout << "DELETE" << std::endl;
	else
		send(newsocket, "HTTP/1.1 405 Method Not Allowed\r\n\r\n", 36, 0);
	send(newsocket, answer.c_str(), answer.size(), 0);
	close(newsocket);
}

void VirtualServer::launch()
{
	fds[0].fd = getSocket()->getSocket(); //aggiungi il socket principale
	fds[0].events = POLLIN; // Aggiungi gli eventi di lettura e scrittura
	for (int i = 1; i < MAX_EVENTS; i++)
		fds[i].fd = -1; // Inizializza tutti gli altri elementi a -1
	while (true)
	{
		int ready = poll(fds, MAX_EVENTS, 50); //50 millisecondi di timeout
		if (ready == -1)
		{
			std::cerr << "poll failed" << std::endl;
			break;
		}
		if (ready == 0) //nessun evento pronto
			continue;
		for(int i = 0; i < MAX_EVENTS; i++)
		{
			if(fds[i].revents != 0)
			{
				if(fds[i].revents & POLLIN)
				{
					cout << "Accepting..." << endl;
					Accepter();
					cout << "Accepted" << endl;
					cout << "Handling..." << endl;
					Handler();
					cout << "Handled" << endl;
				}
				if(fds[i].revents & POLLOUT)
				{
					Responder();
					std::cout << "Done" << std::endl;
					bzero(buffer, 30000);
					//resetto per indicare che è disponibile per la prossima connessione
					fds[i].fd = -1;
				}
				fds[i].revents = 0; //resetto gli eventi
			}
		}
	}
}
