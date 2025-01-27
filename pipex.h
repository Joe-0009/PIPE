#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <limits.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

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
	char	*temp;
	char	**paths;
	int		i;
	char	*cmd_path;
}			t_pipex;

/* Main process functions */
void		first_process(char **av, char **envp, t_pipex *pipex);
void		second_process(char **av, char **envp, t_pipex *pipex);
void		after_execution(t_pipex *pipex);
void		get_paths_cmds(char **av, char **envp, t_pipex *pipex);

/* Utils functions */
void		init_pipex(t_pipex *pipex);
void		free_paths(char **paths);
void		exit_process(t_pipex *pipex, char *error_msg);
int			validate_command(char **cmd);
void		check_args(int ac, char **av);

/* helpers */
void		ft_error(char *str);
void		check_args(int ac, char **av);
int			validate_command(char **cmd);
void		init_pipex(t_pipex *pipex);
void		free_paths(char **paths);
char		**ft_split(char *str, char *charset);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, int i);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
#endif