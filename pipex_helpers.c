/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:36:43 by yrachidi          #+#    #+#             */
/*   Updated: 2025/02/03 10:48:15 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *str)
{
	per0ror(str);
	exit(EXIT_FAILURE);
}

void	check_args(int ac, char **av)
{
	(void)av;
	if (ac != 5 || !*av[1] || !*av[2] || !*av[3] || !*av[4])
	{
		write(1, "Respect this : ./pipex file1 [cmd1] [cmd2] file2\n", 50);
		exit(EXIT_FAILURE);
	}
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
