#include "pipex.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;

	p = (unsigned char *)b;
	while (len > 0)
	{
		p[len - 1] = c;
		len--;
	}
	return (b);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	i = 0;
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	while (i < n && d != s)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	void	*ptr;
	size_t	total_size;

	if (num_elements != 0 && SIZE_MAX / num_elements < element_size)
		return (NULL);
	total_size = num_elements * element_size;
	if (total_size == 0)
		total_size = 1;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, total_size);
	return (ptr);
}
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	len1 = 0;
	len2 = 0;
	if (s1 != NULL)
		len1 = ft_strlen(s1);
	if (s2 != NULL)
		len2 = ft_strlen(s2);
	str = ft_calloc(len1 + len2 + 1, 1);
	if (!str)
		return (NULL);
	if (s1)
		ft_memcpy(str, s1, len1);
	if (s2)
		ft_memcpy(str + len1, s2, len2);
	return (str);
}
