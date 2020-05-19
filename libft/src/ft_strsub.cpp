#include <string.h>
#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*tmp;

	if (!(tmp = (char *)ft_memalloc(len + 1)) || !s)
		return (NULL);
	tmp = strncpy(tmp, (char *)s + start, len);
	return (tmp);
}
