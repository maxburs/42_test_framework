#include <libft.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <framework.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void		handle_signal(int status)
{
	if (status == SIGSEGV)
	{
		INVERSE;
		RED;
		ft_putstr("SEGV");
	}
	else if (status == SIGBUS)
	{
		BOLD;
		RED;
		ft_putstr("BUSE");
	}
	else
	{
		ft_putstr("unknown signal with code: ");
		ft_putnbr(status);
	}
	RESET;
}

static void		handle_exit(int *failed, int status)
{
	if (status == 0)
	{
		GREEN;
		ft_putstr("OK");
	}
	else if (status == -1)
	{
		RED;
		ft_putstr("KO");
		*failed = *failed + 1;
	}
	else
	{
		ft_putstr("unknown function result with code: ");
		ft_putnbr(status);
		*failed = *failed + 1;
	}
	RESET;
}

void			print_result(int total, int failed)
{
	ft_putnbr(total - failed);
	ft_putstr("/");
	ft_putnbr(total);
	ft_putstr(" tests passed\n");
}

void				test(_Bool last, char const *name,
									int (*test)(void), char *output)
{
	static int	total;
	static int	failed;
	int		status;
	pid_t	f;
	int		pipefd[2];

	char	buff[1024];
	int		ret;

	total++;
	ft_putstr(" > ");
	ft_putstr(name);
	ft_putstr(": ");

	pipe(pipefd);
	f = fork();
	if (f == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		exit((*test)());
	}
	else if (f == -1)
	{
		ft_putstr("FORK FAILED");
	}
	close(pipefd[1]);
	wait(&status);
	if (WIFSIGNALED(status))
		handle_signal(WTERMSIG(status));
	else if (WIFEXITED(status))
		handle_exit(&failed, (int)(char)WEXITSTATUS(status));
	else
		ft_putstr("unknown wait result");
	ft_putchar('\n');
	if (output == NULL)
	{
		ft_putstr("output found when null goes here\n");
	}
	if (last)
	{
		print_result(total, failed);
		total = 0;
		failed = 0;
	}
	ret = read(pipefd[0], &buff, 1023);
	buff[ret] = '\0';
	printf("result:\n%s\n", buff);
	close(pipefd[0]);
}
