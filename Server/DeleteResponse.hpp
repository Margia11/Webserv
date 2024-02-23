/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 14:28:37 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/23 14:51:11 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETERESPONSE_HPP
#define DELETERESPONSE_HPP

#include "Response.hpp"
#include "VirtualServer.hpp"
#include "CGI.hpp"
#include <dirent.h>

class DeleteResponse : public Response
{
	public:
		DeleteResponse();
		~DeleteResponse();
		std::string answer(ParserRequest *parser, VirtualServer *vs);
};




#endif
