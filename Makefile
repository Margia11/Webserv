# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/10 14:45:43 by andreamargi       #+#    #+#              #
#    Updated: 2024/03/07 11:22:59 by andreamargi      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Webserv

CC = c++

FLAGS = -std=c++98 -Wall -Wextra -Werror

MAKEFLAGS += --silent

FILES = Socket/BaseSocket.cpp Socket/BindingSocket.cpp \
		Socket/ListeningSocket.cpp Socket/ConnectingSocket.cpp \
		Server/MainServer.cpp Server/SimpleServer.cpp \
		main.cpp Parser/ParserConf.cpp Server/VirtualServer.cpp \
		Parser/ParserRequest.cpp Response/GetResponse.cpp \
		Response/Response.cpp Response/PostResponse.cpp Server/utils.cpp \
		Response/CGI.cpp Response/DeleteResponse.cpp \

OBJ=$(FILES:.cpp=.o)

%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(FILES) -o $(NAME)
	@echo "\033[32mCompiled OK!\033[0m"

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
