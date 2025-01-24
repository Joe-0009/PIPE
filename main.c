#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
# include <stdint.h>

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

int	check_sep(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

int	count_word(char *str, char *charset)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && check_sep(str[i], charset))
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && !check_sep(str[i], charset))
			i++;
	}
	return (count);
}

int	ft_strlens(char *str, char *charset)
{
	int	i;

	i = 0;
	while (str[i] && !check_sep(str[i], charset))
		i++;
	return (i);
}

char	*ft_strdup(char *str, char *charset)
{
	int		len_word;
	int		i;
	char	*copy;

	i = 0;
	len_word = ft_strlens(str, charset);
	copy = (char *)malloc(sizeof(char) * (len_word + 1));
	while (i < len_word)
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	**ft_split(char *str, char *charset)
{
	char	**strs;
	int		i;

	i = 0;
	strs = (char **)malloc(sizeof(char *) * (count_word(str, charset) + 1));
	while (*str != '\0')
	{
		while (*str != '\0' && check_sep(*str, charset))
			str++;
		if (*str != '\0')
		{
			strs[i] = ft_strdup(str, charset);
			i++;
		}
		while (*str && !check_sep(*str, charset))
			str++;
	}
	strs[i] = 0;
	return (strs);
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
	if (needle_len > ft_strlen(haystack))
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

void	ft_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	check_args(int ac, char **av)
{
	if (ac != 5)
		exit(EXIT_FAILURE);
}

// ./pipex file1 cmd1 cmd2 file2
// PATH=/home/yrachidi/bin:/usr/local/sbin:/usr/local/bin
//:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
int	main(int ac, char **av, char **envp)
{
	int		pipe_fd[2];
		// fd[0] Read end
		// fd[1] write end
	pid_t	pid1;
	pid_t	pid2;
	int		fd_infile;
	int		fd_outfile;
	char **cmd1;
	char **cmd2;
	char *fullpath;
	char **paths;
	int i;

	check_args(ac, av);
	cmd1 = ft_split(av[2], " 	");
	cmd2 = ft_split(av[3], " 	");
	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH", 4))
		{
			fullpath = ft_strnstr(envp[i], "PATH", 4);
			paths = ft_split(fullpath + 5 , ":");
			break;
		}
	}
	if (pipe(pipe_fd) == -1)
		ft_error("pipe");
	fd_infile = open(av[1], O_RDONLY | O_CREAT, 0644);
	if (fd_infile == -1)
		ft_error("open");
	pid1 = fork();
	if (pid1 == -1)
		ft_error("fork");
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		dup2(fd_infile, STDIN_FILENO);
		close(fd_infile);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		i = -1;
		while (paths[++i])
		{
			if (access(ft_strjoin(ft_strjoin(paths[i], "/"), cmd1[0]), X_OK) != -1)
				execve(ft_strjoin(ft_strjoin(paths[i], "/"), cmd1[0]), cmd1, envp);
		}
		printf("CMD not found");
	}
	if (pid1 != 0)
	{
		
		pid2 = fork();
		if (pid2 == -1)
			ft_error("fork");
		if (pid2 == 0)
		{
			close(pipe_fd[1]);
			fd_outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd_outfile, STDOUT_FILENO);
			close(fd_outfile);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			i = -1;
			while (paths[++i])
			{
				if (access(ft_strjoin(ft_strjoin(paths[i], "/"), cmd1[0]), X_OK) != -1)
					execve(ft_strjoin(ft_strjoin(paths[i], "/"), cmd1[0]), cmd1, envp);
			}
			printf("CMD not found");
		}
	}
	if (pid1 != 0 && pid2 != 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
		close(fd_infile);
	}
	exit(EXIT_SUCCESS);
}
