#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
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

// infile "cmd1" "Cmd2" outfile

int	main(int ac, char **av)
{
	int pipe_fd[2];
	pid_t pid1;
	pid_t pid2;

	check_args(ac, av);
	if (pipe(pipe_fd) == -1)
		ft_error("pipe");

	open(inr)
	pid1 = fork();
	if (pid1 == -1)
		ft_error("fork");
	if (pid1 == 0)
	{

	}
	if (pid1 != 0)
	{
		pid2 = fork();
		if (pid2 == -1)
			ft_perror("fork");
		if (pid2 == 0)
		{
			
		}
	}
    if (pid1 == 0 && pid2 == 0)
	    waitpid();
}


fd[0] = t789
fd[1] = 456789