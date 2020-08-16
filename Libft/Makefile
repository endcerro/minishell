#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hpottier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 14:27:31 by hpottier          #+#    #+#              #
#    Updated: 2020/08/16 19:54:45 by hpottier         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	libft.a
SRC			=	ft_bzero.c \
				ft_isalnum.c \
				ft_isalpha.c \
				ft_isascii.c \
				ft_isdigit.c \
				ft_islower.c \
				ft_isprint.c \
				ft_isupper.c \
				ft_memccpy.c \
				ft_memcpy.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_memmove.c \
				ft_memset.c \
				ft_strchr.c \
				ft_strlen.c \
				ft_strrchr.c \
				ft_tolower.c \
				ft_toupper.c \
				ft_strcmp.c \
				ft_strncmp.c \
				ft_strlcpy.c \
				ft_strlcat.c \
				ft_strnstr.c \
				ft_atoi.c \
				ft_itoa.c \
				ft_itoa_base.c \
				ft_isspace.c \
				ft_calloc.c \
				ft_strdup.c \
				ft_putchar_fd.c \
				ft_putstr_fd.c \
				ft_putendl_fd.c \
				ft_putsendl_fd.c \
				ft_putnbr_fd.c \
				ft_putchar.c \
				ft_putstr.c \
				ft_putendl.c \
				ft_putsendl.c \
				ft_putnbr.c \
				ft_strjoin.c \
				ft_substr.c \
				ft_strtrim.c \
				ft_split.c \
				ft_itoa.c \
				ft_strmapi.c \
				get_next_line.c \
				ft_printh.c

OBJ			=	$(SRC:.c=.o)

CFLAGS		=	-Wall -Wextra -Werror -march=native -O3 -pipe -g3 -fsanitize=address

AC			=	ar rc
RAN			=	ranlib
CC			=	clang
RM			=	rm -f
ECHO		=	echo

$(NAME)	:		$(OBJ)
#				@$(ECHO) "\033[38;5;208m> Building Libft\033[38;5;125m"
				$(AC) $(NAME) $(OBJ)
				$(RAN) $(NAME)
				@$(ECHO) "\033[32m> Libft compiled\033[0m"

clean	:
				-@$(RM) $(OBJ)
#				@$(ECHO) "\033[94m> Libft Objects removed\033[0m"

all		:		$(NAME)

fclean	:		clean
				-@$(RM) $(NAME)
#				@$(ECHO) "\033[94m> Libft cleaned all\033[0m"

re		:		fclean all

.PHONY	:		all clean re fclean

%.o		:		%.c
#				@$(ECHO) "\033[38;5;208m> Compiling Libft\033[38;5;125m"
				$(CC) -I. $(CFLAGS) -o $@ -c $<
