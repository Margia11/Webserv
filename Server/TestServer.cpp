/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:41 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/14 11:01:44 by andreamargi      ###   ########.fr       */
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
}

void TestServer::Handler()
{
	std::cout << buffer << std::endl;
}

void TestServer::Responder()
{
	const char *hello = "Hello from server";
	write(newsocket, hello, strlen(hello));
	close(newsocket);
}

void TestServer::launch()
{
	while (true)
	{
		Accepter();
		Handler();
		Responder();
		std::cout << "Done" << std::endl;
	}
}
