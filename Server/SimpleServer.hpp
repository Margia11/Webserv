/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:16:11 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/14 11:07:15 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include "../Networking/Networking.hpp"
#include <unistd.h>

class Server
{
	private :
		ListeningSocket *socket;
		virtual void Accepter() = 0;
		virtual void Handler() = 0;
		virtual void Responder() = 0;
	public :
		Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
		virtual void launch() = 0;
		ListeningSocket *getSocket();
};

#endif
