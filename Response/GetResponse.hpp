/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:41:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/07 11:18:40 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETRESPONSE_HPP
#define GETRESPONSE_HPP

#include <cstring>
#include <cstdlib>
#include <sstream>
#include "Response.hpp"
#include "CGI.hpp"
#include "../Server/VirtualServer.hpp"

class GetResponse : public Response
{
	public:
		GetResponse();
		~GetResponse();
		std::string answer(ParserRequest *parser, VirtualServer *vs);
	std::string generateAutoindex(std::string root, std::string uri);
};




#endif
