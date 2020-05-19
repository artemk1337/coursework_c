#include "libft.h"

void	ft_putstr(const char *s)
{
	int i;

	if (!s)
		return ;
	i = strlen((char *)s);
	write(1, s, i);
}
