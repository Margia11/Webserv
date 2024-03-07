/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 14:28:37 by andreamargi       #+#    #+#             */
/*   Updated: 2024/03/07 11:19:43 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETERESPONSE_HPP
#define DELETERESPONSE_HPP

#include <dirent.h>
#include "Response.hpp"
#include "CGI.hpp"
#include "../Server/VirtualServer.hpp"

class DeleteResponse : public Response
{
	public:
		DeleteResponse();
		~DeleteResponse();
		std::string answer(ParserRequest *parser, VirtualServer *vs);
};




#endif
