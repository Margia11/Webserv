/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:14:38 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 14:06:01 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port,
	u_long interface, int bklg) : BindingSocket(domain, service, protocol, port, interface)
{
	backlog = bklg;
	startListening();
	test_connection(listening);
}

void ListeningSocket::startListening()
{
	listening = listen(getSocket(), backlog);
}

int ListeningSocket::getBacklog()
{
	return backlog;
}

int ListeningSocket::getListening()
{
	return listening;
}

