#include "pipex.h"

char	*ft_strchr(const char *s, int i)
{
	char	c;

	c = (char)i;
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	needle_len;

	if (!haystack && len == 0)
		return (NULL);
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)haystack);
	if (needle_len > (size_t)ft_strlen(haystack))
		return (NULL);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && j < needle_len && i + j < len)
			j++;
		if (j == needle_len)
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}
