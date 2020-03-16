#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hpottier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 14:27:31 by hpottier          #+#    #+#              #
#    Updated: 2020/01/04 18:18:49 by hpottier         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	minishell

DIRSRC		=	Sources
DIROBJ		=	Objets
INCLUDE		=	Includes
SUB_MAKE	=	Libft
INC_LIB		=	-L./Libft -lft

SRC			=	main.c

OBJ			=	$(SRC:.c=.o)
OBJS		=	$(OBJ:%=$(DIROBJ)/%)

CFLAGS		=	-Wall -Wextra -Werror

CC			=	gcc
RM			=	rm -f
ECHO		=	echo

$(NAME)	:		$(OBJ)
				@$(ECHO) "\033[38;5;208m> Building Libft\033[38;5;125m"
				$(MAKE) -C $(SUB_MAKE)
				$(CC) -I./$(INCLUDE) $(INC_LIB) $(CFLAGS) -o $(NAME) $(OBJS)
				@$(ECHO) "\033[32m> Minishell compiled\033[0m"

clean	:
				@$(MAKE) clean -C $(SUB_MAKE)
				@$(ECHO) "\033[94m> Libft Objects removed\033[0m"
				-@$(RM) $(OBJS)
				@$(ECHO) "\033[94m> Minishell Objects removed\033[0m"

all		:		$(NAME)

#color	:
#				@$(ECHO) "\033[38;5;208m> Compiling Libft\033[38;5;125m"

fclean	:		clean
				@$(MAKE) fclean -C $(SUB_MAKE)
				@$(ECHO) "\033[94m> Libft cleaned all\033[0m"
				-@$(RM) $(NAME)
				@$(ECHO) "\033[94m> Minishell cleaned all\033[0m"

re		:		fclean all

.PHONY	:		all clean re fclean color

%.o		:		$(DIRSRC)/%.c
				@$(ECHO) "\033[38;5;208m> Compiling Minishell\033[38;5;125m"
				$(CC) -I./$(INCLUDE) $(INC_LIB) $(CFLAGS) -o $(DIROBJ)/$@ -c $<
