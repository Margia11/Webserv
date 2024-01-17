/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindingSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 18:16:08 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/05 11:14:31 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINDINGSOCKET_HPP
#define BINDINGSOCKET_HPP

#include "BaseSocket.hpp"

//BindingSocket si occupa di stabilire la connessione di tipo bind
//quindi con vincolo di indirizzo
//bind() infatti associa un indirizzo IP e una porta ad un socket
class BindingSocket : public Socket
{
	public :
		BindingSocket(int domain, int service, int protocol, int port, u_long interface);
		int establishConnection(int sock, sockaddr_in address);
};

#endif
