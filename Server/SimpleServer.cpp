/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:02 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 16:22:40 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleServer.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
	socket = new ListeningSocket(domain, service, protocol, port, interface, bklg);
}

ListeningSocket *Server::getSocket()
{
	return (socket);
}
