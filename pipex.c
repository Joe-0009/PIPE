/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:36:51 by yrachidi          #+#    #+#             */
/*   Updated: 2025/02/05 16:27:26 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	reset_pipex(t_pipex *pipex)
{
	pipex->cmd1 = NULL;
	pipex->cmd2 = NULL;
	pipex->paths = NULL;
	pipex->fullpath = NULL;
	pipex->cmd_path = NULL;
}

void	exit_process(t_pipex *pipex, char *error_msg)
{
	ft_error(error_msg);
	after_execution(pipex);
	exit(EXIT_FAILURE);
}

void	get_paths_cmds(char **av, char **envp, t_pipex *pipex)
{
	pipex->cmd1 = ft_split(av[2], " \t");
	pipex->cmd2 = ft_split(av[3], " \t");
	while (envp[++pipex->i])
	{
		if (ft_strnstr(envp[pipex->i], "PATH", 4))
		{
			pipex->fullpath = ft_strnstr(envp[pipex->i], "PATH", 4);
			pipex->paths = ft_split(pipex->fullpath + 5, ":");
			return ;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	check_args(ac, av);
	init_pipex(&pipex);
	get_paths_cmds(av, envp, &pipex);
	if (pipe(pipex.pipe_fd) == -1)
		exit_process(&pipex, "pipe");
	pipex.pid1 = fork();
	if (pipex.pid1 == -1)
		exit_process(&pipex, "fork");
	if (pipex.pid1 == 0)
		first_process(av, envp, &pipex);
	pipex.pid2 = fork();
	if (pipex.pid2 == -1)
		exit_process(&pipex, "fork");
	if (pipex.pid2 == 0)
		second_process(av, envp, &pipex);
	after_execution(&pipex);
	exit(EXIT_SUCCESS);
}
