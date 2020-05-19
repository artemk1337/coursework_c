#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <cstdlib> //stdlib.h
# include <cstring> //string.h
# include "get_next_line.h"

typedef	struct	s_vec
{
	size_t			max_size;
	size_t			curr_size;
	char			*vec;
}				t_vec;

unsigned long	ft_atoul(const char *str);
int				ft_isdigit(int c);
int				ft_isdigitstr(char *str);
void			*ft_memalloc(size_t size);
void			ft_bzero(void *s, size_t n);
void			ft_memdel(void **ap);
void			ft_strdel(char **as);
int				ft_strequ(char const *s1, char const *s2);
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			**ft_strsplit(char const *s, char c);
void			ft_putchar(char c);
void			ft_putstr(char const *s);
void			ft_putnbr(int n);
int				ft_isspace(int c);

t_vec			*ft_create_vec(int len);
t_vec			*ft_realloc_vec(t_vec *vec);
t_vec			*ft_put_vec(t_vec *vec, char *line);

#endif
