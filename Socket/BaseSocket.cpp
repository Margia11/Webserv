/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:46:10 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/07 16:06:42 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseSocket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	//definizione struttura
	address.sin_family = domain;
	address.sin_port = htons(port); //porta(htons converte un intero da formato host a formato network(16 bit)))
	address.sin_addr.s_addr = htonl(interface); //indirizzo ip(htonl converte un intero da formato host a formato network(32 bit)))
	//creazione socket e connessione
	sock = socket(domain, service, protocol); //creazione socket
	test_connection(sock);
}

void Socket::test_connection(int test)
{
	if (test < 0)
	{
		perror("Failed to connect...");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in Socket::getAddress()
{
	return address;
}

int Socket::getSocket()
{
	return sock;
}

int Socket::getConnection()
{
	return connection;
}

void Socket::setConnection(int con)
{
	connection = con;
}
