#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*join_str;
	size_t	i;
	size_t	j;
	size_t	len1;
	size_t	len2;

	if (s1 && s2)
	{
		len1 = strlen((char *)s1);
		len2 = strlen((char *)s2);
		i = -1;
		j = 0;
		if (!(join_str = (char *)ft_memalloc(sizeof(char) * len1 + len2 + 1)))
			return (NULL);
		while (++i < len1)
			join_str[i] = s1[i];
		while (j < len2)
			join_str[i++] = s2[j++];
		join_str[i] = '\0';
		return (join_str);
	}
	return (NULL);
}
