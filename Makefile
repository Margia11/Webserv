# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: andreamargiacchi <andreamargiacchi@stud    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/10 14:45:43 by andreamargi       #+#    #+#              #
#    Updated: 2024/02/19 17:11:22 by andreamargi      ###   ########.fr        #
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
		Parser/ParserRequest.cpp Server/GetResponse.cpp \
		Server/Response.cpp Server/PostResponse.cpp Server/utils.cpp \
		Server/CGI.cpp \

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
