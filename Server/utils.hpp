/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 10:51:10 by andreamargi       #+#    #+#             */
/*   Updated: 2024/02/15 17:42:18 by andreamargi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "fstream"
#include <sys/stat.h>

#include <cstring>

u_int32_t string_to_byte_order(const char *ip_addr);
std::pair<std::string, int>	toHostPort(std::string raw);
std::string getWholeFile(const std::string &path);
std::string numberToString(int n);
bool isValidFile(const char* path);

#endif
