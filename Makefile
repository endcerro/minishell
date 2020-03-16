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
OBJD		=	Objets
INCLUDE		=	Includes
SUB_MAKE	=	Libft
LIB			=	$(SUB_MAKE)/libft.a

SRC			=	main.c 				\
				get_commands.c

OBJ			=	$(SRC:.c=.o)
OBJS		=	$(OBJ:%=$(OBJD)/%)

CFLAGS		=	-Wall -Wextra -Werror -g

CC			=	gcc
RM			=	rm -f
ECHO		=	echo

$(NAME)	:		$(LIB) $(OBJD) $(OBJ)
				@$(ECHO) "\033[32m> Minishell built\033[0m"
				@$(ECHO) "\033[38;5;208m> Compiling Minishell\033[38;5;125m"
				$(CC) -I./$(INCLUDE) -I./$(SUB_MAKE) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)
				@$(ECHO) "\033[32m> Minishell compiled\033[0m"

$(LIB)	:
				@$(ECHO) "\033[38;5;208m> Compiling Libft\033[38;5;125m"
				@($(MAKE) -s -C $(SUB_MAKE))

clean	:
				@($(MAKE) clean -s -C $(SUB_MAKE))
				@$(ECHO) "\033[94m> Libft Objects removed\033[0m"
				-@$(RM) $(OBJS)
				@$(ECHO) "\033[94m> Minishell Objects removed\033[0m"

$(OBJD)	:
				mkdir $(OBJD)

all		:		$(NAME)

fclean	:		clean
				@($(MAKE) fclean -s -C $(SUB_MAKE))
				@$(ECHO) "\033[94m> Libft cleaned all\033[0m"
				-@$(RM) $(NAME)
				@$(ECHO) "\033[94m> Minishell cleaned all\033[0m"

re		:		fclean all

.PHONY	:		all clean re fclean color

%.o		:		$(DIRSRC)/%.c
				@$(ECHO) "\033[38;5;208m> Building Minishell\033[38;5;125m"
				$(CC) -I./$(INCLUDE) -I./$(SUB_MAKE) $(CFLAGS) -o $(OBJD)/$@ -c $<
