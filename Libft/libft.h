/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 11:05:22 by hpottier          #+#    #+#             */
/*   Updated: 2020/03/20 22:23:18 by edal--ce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>

void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
size_t			ft_strlen(const char *s);
int				ft_isupper(int c);
int				ft_islower(int c);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcat(char *dest, const char *src, size_t size);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
char			*ft_strnstr(const char *str, const char *to_find, size_t len);
int				ft_atoi(const char *str);
char			*ft_itoa(int nb);
int				ft_isspace(int c);
void			*ft_calloc(size_t count, size_t size);
char			*ft_strdup(const char *s1);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putsendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_putchar(const char c);
void			ft_putstr(const char *s);
void			ft_putendl(const char *s);
void			ft_putsendl(const char *s);
void			ft_putnbr(int n);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strjoinf1(char *s1, char const *s2);
char			*ft_strjoinf2(char const *s1, char *s2);
char			*ft_strjoinft(char *s1, char *s2);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_substrf1(char *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
char			**ft_split(char const *s, char c);
char			*ft_itoa(int n);
char			*ft_itoa_base(int nb, char *base);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				get_next_line(int fd, char **line);

#endif
