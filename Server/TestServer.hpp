/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:18:33 by andreamargi       #+#    #+#             */
/*   Updated: 2024/01/22 14:15:14 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTSERVER_HPP
#define TESTSERVER_HPP

#include "SimpleServer.hpp"
#include <stdio.h>
#include <poll.h>

class TestServer : public Server
{
	private :
		char buffer[30000];
		int newsocket;
		static const int MAX_EVENTS = 10;
		struct pollfd fds[MAX_EVENTS];
		void Accepter();
		void Handler();
		void Responder();
	public :
		TestServer();
		void launch();
};

#endif
