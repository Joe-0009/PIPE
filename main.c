#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
	{
		ft_memcpy(str, s1, len1);
		free(s1);
	}
	if (s2)
	{
		ft_memcpy(str + len1, s2, len2);
		free(s2);
	}
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
	{
		ft_error("arguments");
		exit(EXIT_FAILURE);
	}
}

// ./pipex file1 cmd1 cmd2 file2
// PATH=/home/yrachidi/bin:/usr/local/sbin:/usr/local/bin
//:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin

typedef struct s_pipex
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		fd_infile;
	int		fd_outfile;
	char	**cmd1;
	char	**cmd2;
	char	*fullpath;
	char	**paths;
	int		i;
	char	*cmd_path;

}			t_pipex;

void	get_paths_cmds(char **av, char **envp, t_pipex *pipex)
{
	pipex->cmd1 = ft_split(av[2], " 	");
	pipex->cmd2 = ft_split(av[3], " 	");
	pipex->i = -1;
	while (envp[++pipex->i])
	{
		if (ft_strnstr(envp[pipex->i], "PATH", 4))
		{
			pipex->fullpath = ft_strnstr(envp[pipex->i], "PATH", 4);
			pipex->paths = ft_split(pipex->fullpath + 5, ":");
			break ;
		}
	}
}

void	first_process(char **av, char **envp, t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	dup2(pipex->fd_infile, STDIN_FILENO);
	close(pipex->fd_infile);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
	if (pipex->cmd1[0][0] == '/')
	{
		if (access(pipex->cmd1[0], X_OK) != -1)
			execve(pipex->cmd1[0], pipex->cmd1, envp);
		else
			ft_error("access");
	}
	else
	{
		pipex->i = -1;
		while (pipex->paths && pipex->paths[++pipex->i])
		{
			pipex->cmd_path = ft_strjoin(ft_strjoin(pipex->paths[pipex->i],
						"/"), pipex->cmd1[0]);
			if (access(pipex->cmd_path, X_OK) != -1)
				execve(pipex->cmd_path, pipex->cmd1, envp);
		}
		ft_error("Command 1 not found");
	}
}

void	second_process(char **av, char **envp, t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	pipex->fd_outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(pipex->fd_outfile, STDOUT_FILENO);
	close(pipex->fd_outfile);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
	if (pipex->cmd2[0][0] == '/')
	{
		if (access(pipex->cmd2[0], X_OK) != -1)
			execve(pipex->cmd2[0], pipex->cmd2, envp);
		else
			ft_error("access");
	}
	else
	{
		pipex->i = -1;
		while (pipex->paths && pipex->paths[++pipex->i])
		{
			pipex->cmd_path = ft_strjoin(ft_strjoin(pipex->paths[pipex->i],
						"/"), pipex->cmd2[0]);
			if (access(pipex->cmd_path, X_OK) != -1)
				execve(pipex->cmd_path, pipex->cmd2, envp);
		}
	}
	ft_error("Command 2 not found");
}
void	after_execution(t_pipex *pipex)
{
	int i;

	i = -1;
	if (pipex->pid1 == 0)
	{
		free(pipex->cmd_path);
		free(pipex->paths);
		free(pipex->cmd1);
		free(pipex->cmd2);
	}
	if (pipex->pid2 == 0)
	{
		free(pipex->cmd_path);
		free(pipex->paths);
		free(pipex->cmd1);
		free(pipex->cmd2);
	}	
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, NULL, 0);
	close(pipex->fd_infile);
	while(pipex->paths[++i])
		free(pipex->paths[++i]);
	free(pipex->paths);
	free(pipex->cmd1);
	free(pipex->cmd2);
}
int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	check_args(ac, av);
	get_paths_cmds(av, envp, &pipex);
	if (pipe(pipex.pipe_fd) == -1)
		ft_error("pipe");
	pipex.fd_infile = open(av[1], O_RDONLY);
	if (pipex.fd_infile == -1)
		ft_error("open");
	pipex.pid1 = fork();
	if (pipex.pid1 == -1)
		ft_error("fork");
	if (pipex.pid1 == 0)
		first_process(av, envp, &pipex);
	if (pipex.pid1 != 0)
	{
		pipex.pid2 = fork();
		if (pipex.pid2 == -1)
			ft_error("fork");
		if (pipex.pid2 == 0)
			second_process(av, envp, &pipex);
	}
	if (pipex.pid1 != 0 && pipex.pid2 != 0)
		after_execution(&pipex);
	exit(EXIT_SUCCESS);
}
