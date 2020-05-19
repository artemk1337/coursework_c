#include "libft.h"

int	ft_isspace(int c)
{
	if ((c == '\t') || (c == ' ') || (c == '\f')
		|| (c == '\n') || (c == '\v') || (c == '\r'))
		return (1);
	else
		return (0);
}
