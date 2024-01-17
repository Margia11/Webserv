/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:14:17 by andreamargi       #+#    #+#             */
/*   Updated: 2023/11/13 14:06:06 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

class ListeningSocket : public BindingSocket
{
	private :
		int backlog;
		int listening;
	public :
		ListeningSocket(int domain, int service, int protocol, int port,
			u_long interface, int bklg);
		void startListening();
		int getBacklog();
		int getListening();
};

#endif
