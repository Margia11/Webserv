/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:40 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 11:42:11 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectingSocket.hpp"

ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) :
	Socket(domain, service, protocol, port, interface)
{
	setConnection(establishConnection(getSocket(), getAddress()));
	test_connection(getConnection());
}


int ConnectingSocket::establishConnection(int sock, sockaddr_in address)
{
	return (connect(sock, (struct sockaddr *)&address, sizeof(address)));
}
