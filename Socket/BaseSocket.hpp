/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:46:01 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/05 11:21:39 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASESOCKET_HPP
#define BASESOCKET_HPP

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

//BaseSocket si occupa di creare e configurare il socket che è visto come un
//file descriptor che viene usato per stabilire la connessione
class Socket
{
	private :
		struct sockaddr_in address;
		int sock;
		int connection;

	public :
		Socket(int domain, int service, int protocol, int port, u_long interface);
		virtual int establishConnection(int sock, sockaddr_in address) = 0;
		void test_connection(int test);
		struct sockaddr_in getAddress();
		int getSocket();
		int getConnection();
		void setConnection(int con);
};

#endif
