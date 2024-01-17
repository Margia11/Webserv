/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:36:17 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 13:49:57 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "BaseSocket.hpp"

//ConnectinSocket si occupa della connessione effettiva
//tramite la funzione connect()
class ConnectingSocket : public Socket
{
	public :
		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
		int establishConnection(int sock, sockaddr_in address);
};


#endif
