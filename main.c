#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	char **cmds;


	check_args(ac, av);
	cmds = ft_split();
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
