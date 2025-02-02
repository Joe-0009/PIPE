/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrachidi <yrachidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:36:38 by yrachidi          #+#    #+#             */
/*   Updated: 2025/02/02 20:06:31 by yrachidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	close_and_dup_first(char **av, t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	pipex->fd_infile = open(av[1], O_RDONLY);
	if (pipex->fd_infile == -1)
		exit_process(pipex, av[1]);
	dup2(pipex->fd_infile, STDIN_FILENO);
	close(pipex->fd_infile);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
}

int	is_path(char *cmd)
{
	while (*cmd)
	{
		if (*cmd == '/')
			return (1);
		cmd++;
	}
	return (0);
}

void	first_process(char **av, char **envp, t_pipex *pipex)
{
	close_and_dup_first(av, pipex);
	if (!validate_command(pipex->cmd1))
		exit_process(pipex, pipex->cmd1[0]);
	if (is_path(pipex->cmd1[0]))
	{
		if (access(pipex->cmd1[0], X_OK) != -1)
			execve(pipex->cmd1[0], pipex->cmd1, envp);
		exit_process(pipex, pipex->cmd1[0]);
	}
	pipex->i = -1;
	while (pipex->paths && pipex->paths[++pipex->i])
	{
		pipex->temp = ft_strjoin(pipex->paths[pipex->i], "/");
		pipex->cmd_path = ft_strjoin(pipex->temp, pipex->cmd1[0]);
		free(pipex->temp);
		if (access(pipex->cmd_path, X_OK) != -1)
			execve(pipex->cmd_path, pipex->cmd1, envp);
		free(pipex->cmd_path);
		pipex->cmd_path = NULL;
	}
	write(2, pipex->cmd1[0], ft_strlen(pipex->cmd1[0]));
	write(2, ": Command not found\n", 21);
	after_execution(pipex);
	exit(1);
}

static void	close_and_dup_second(char **av, t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	pipex->fd_outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_outfile == -1)
		exit_process(pipex, av[4]);
	dup2(pipex->fd_outfile, STDOUT_FILENO);
	close(pipex->fd_outfile);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
}

void	second_process(char **av, char **envp, t_pipex *pipex)
{
	close_and_dup_second(av, pipex);
	if (!validate_command(pipex->cmd2))
		exit_process(pipex, "Command 2 not found");
	if (is_path(pipex->cmd2[0]))
	{
		if (access(pipex->cmd2[0], X_OK) != -1)
			execve(pipex->cmd2[0], pipex->cmd2, envp);
		// after_execution(pipex);
		exit_process(pipex, pipex->cmd2[0]);
	}
	pipex->i = -1;
	while (pipex->paths && pipex->paths[++pipex->i])
	{
		pipex->temp = ft_strjoin(pipex->paths[pipex->i], "/");
		pipex->cmd_path = ft_strjoin(pipex->temp, pipex->cmd2[0]);
		free(pipex->temp);
		if (access(pipex->cmd_path, X_OK) != -1)
			execve(pipex->cmd_path, pipex->cmd2, envp);
		free(pipex->cmd_path);
		pipex->cmd_path = NULL;
	}
	write(2, pipex->cmd2[0], ft_strlen(pipex->cmd2[0]));
	write(2, ": Command not found\n", 21);
	after_execution(pipex);
	exit(1);
}

void	after_execution(t_pipex *pipex)
{
	if (pipex->fd_infile != -1)
		close(pipex->fd_infile);
	if (pipex->fd_outfile != -1)
		close(pipex->fd_outfile);
	if (pipex->cmd_path)
	{
		free(pipex->cmd_path);
		pipex->cmd_path = NULL;
	}
	if (pipex->paths)
		free_paths(pipex->paths);
	if (pipex->cmd1)
		free_paths(pipex->cmd1);
	if (pipex->cmd2)
		free_paths(pipex->cmd2);
	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->pid1)
		waitpid(pipex->pid1, NULL, 0);
	if (pipex->pid2)
		waitpid(pipex->pid2, NULL, 0);
}
