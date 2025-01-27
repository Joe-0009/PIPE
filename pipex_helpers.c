#include "pipex.h"

void	ft_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	check_args(int ac, char **av)
{
	(void)av;
	if (ac != 5)
		ft_error("arguments");
}

int	validate_command(char **cmd)
{
	char	*ptr;

	if (!cmd || !cmd[0])
		return (0);
	ptr = cmd[0];
	while (*ptr)
	{
		if (*ptr != ' ' && *ptr != '\t')
			return (1);
		ptr++;
	}
	return (0);
}

void	init_pipex(t_pipex *pipex)
{
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
	pipex->fd_infile = -1;
	pipex->fd_outfile = -1;
	pipex->cmd1 = NULL;
	pipex->cmd2 = NULL;
	pipex->temp = NULL;
	pipex->paths = NULL;
	pipex->cmd_path = NULL;
	pipex->fullpath = NULL;
	pipex->i = -1;
	pipex->pid1 = 0;
	pipex->pid2 = 0;
}

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}
