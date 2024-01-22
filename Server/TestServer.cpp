/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:41 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/22 14:22:26 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestServer.hpp"

TestServer::TestServer() : Server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	launch();
}

void TestServer::Accepter()
{
	struct sockaddr_in address = getSocket()->getAddress();
	int addrlen = sizeof(address);
	newsocket = accept(getSocket()->getSocket(), (struct sockaddr *)&address,
			(socklen_t*)&addrlen);
	read(newsocket, buffer, 30000);
	std::cout << "Accepted" << std::endl;
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

void TestServer::Handler()
{
	std::cout << buffer << std::endl;
	std::cout << "Handled" << std::endl;
}

void TestServer::Responder()
{
	const char *hello = "Hello from server";
	write(newsocket, hello, strlen(hello));
	close(newsocket);
}

void TestServer::launch()
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
					std::cout << "New connection" << std::endl;
					Accepter();
					Handler();
				}
				if(fds[i].revents & POLLOUT)
				{
					std::cout << "Sending data" << std::endl;
					Responder();
					std::cout << "Done" << std::endl;
					//resetto per indicare che è disponibile per la prossima connessione
					fds[i].fd = -1;
				}
				fds[i].revents = 0; //resetto gli eventi
			}
		}
	}
}

