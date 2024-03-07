/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:42:54 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/07 11:17:59 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTRESPONSE_HPP
#define POSTRESPONSE_HPP

#include <dirent.h>
#include "Response.hpp"
#include "CGI.hpp"
#include "../Server/VirtualServer.hpp"

class PostResponse : public Response
{
	public:
		PostResponse();
		~PostResponse();
		std::string answer(ParserRequest *parser, VirtualServer *vs);
};




#endif
