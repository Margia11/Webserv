/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:41:59 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/20 14:54:35 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETRESPONSE_HPP
#define GETRESPONSE_HPP

#include "Response.hpp"
#include "VirtualServer.hpp"
#include "CGI.hpp"

class GetResponse : public Response
{
	public:
		GetResponse();
		~GetResponse();
		std::string answer(ParserRequest *parser, VirtualServer *vs);
};




#endif
