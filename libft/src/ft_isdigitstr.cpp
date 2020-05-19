#include "libft.h"

int	ft_isdigitstr(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return (1);

}