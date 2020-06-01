#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

extern char	**environ;

int			main(void)
{
	int		pipefd[2];
	char	**argv;

	pipe(pipefd);
	if (fork() == 0)
	{
		argv = (char **)malloc(sizeof(char *) * 2);
		argv[0] = strdup("ls");
		argv[1] = strdup("-la");
		dup2(pipefd[1], 1);
		execve("/bin/ls", argv, environ);
	}
	wait(0);
	close(pipefd[1]);
	if (fork() == 0)
	{
		argv = (char **)malloc(sizeof(char *) * 2);
		argv[0] = strdup("grep");
		argv[1] = strdup("git");
		dup2(pipefd[0], 0);
		execve("/bin/grep", argv, environ);
	}
	wait(0);
	return (0);
}
