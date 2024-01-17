/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindingSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 18:15:51 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 11:32:35 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) :
	Socket(domain, service, protocol, port, interface)
{
	setConnection(establishConnection(getSocket(), getAddress()));
	test_connection(getConnection());
}



int BindingSocket::establishConnection(int sock, sockaddr_in address)
{
	return (bind(sock, (struct sockaddr *)&address, sizeof(address)));
}
