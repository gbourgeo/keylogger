#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: root </var/mail/root>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/08 09:15:41 by root              #+#    #+#              #
#    Updated: 2018/11/08 09:23:53 by root             ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME	= key

SRC_D	= srcs/
SRCS	= main.c get_console.c get_keymaps.c get_keys.c get_keyboard.c
SRCS	+= keylogger.c

OBJ_D	= objs/
OBJS	= $(addprefix $(OBJ_D), $(SRCS:.c=.o))

INCS	= includes/

FLAGS	= -Wall -Werror -Wextra

all: $(OBJ_D) $(NAME)

$(OBJ_D):
	@mkdir -p $@

$(NAME): $(OBJS)
	gcc -o $@ $^

$(OBJ_D)%.o: $(SRC_D)%.c
	gcc -o $@ -c $< $(FLAGS) -I$(INCS)

clean:
	/bin/rm -rf $(OBJ_D)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
