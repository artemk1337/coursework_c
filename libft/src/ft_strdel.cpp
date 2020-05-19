#include "libft.h"

void	ft_strdel(char **as)
{
	if (!as || !*as)
		return ;
	ft_memdel((void **)as);
}
