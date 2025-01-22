#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int	ft_words_count(char const *s, char c)
{
	int	count;
	int	word;

	count = 0;
	word = 0;
	while (*s && *s != '\n')
	{
		if (*s != c && !word)
		{
			word = 1;
			count++;
		}
		else if (*s == c)
		{
			word = 0;
		}
		s++;
	}
	return (count);
}

char	*ft_str_s_dup(char const *s, char c, int *start)
{
	char	*dup;
	int		end;
	int		i;

	end = 0;
	while (s[*start + end] && s[*start + end] != c && s[*start + end] != '\n')
		end++;
	dup = (char *)calloc(end + 1, 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < end)
	{
		dup[i] = s[*start];
		(*start)++;
		i++;
	}
	return (dup);
}

void	ft_free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	strs = (char **)calloc((ft_words_count(s, c) + 1), sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && s[i] != '\n')
	{
		if (s[i] == c && s[i++])
			continue ;
		strs[j] = ft_str_s_dup(s, c, &i);
		if (!strs[j++])
			return (ft_free_strs(strs), NULL);
	}
	strs[j] = NULL;
	return (strs);
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

// infile "Cmd1" "Cmd2" outfile

int	main(int ac, char **av)
{
	int		pipe_fd[2];
		// fd[0] Read end
		// fd[1] write end
	pid_t	pid1;
	pid_t	pid2;
	int		fd_infile;
	int		fd_outfile;

	check_args(ac, av);
	if (pipe(pipe_fd) == -1)
		ft_error("pipe");
	fd_infile = open("infile", O_RDONLY | O_CREAT, 0644);
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
	}
	if (pid1 != 0)
	{
		pid2 = fork();
		if (pid2 == -1)
			ft_error("fork");
		if (pid2 == 0)
		{
			close(pipe_fd[1]);
			fd_outfile = open("outfile", O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(fd_outfile, STDOUT_FILENO);
			close(fd_outfile);
			dup2(pipe_fd[0], STDIN_FILENO);
		}
	}
	if (pid1 != 0 && pid2 != 0)
	{
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
	}
}
