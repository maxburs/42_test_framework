#include <libft.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <framework.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> //NULL
#include <stdbool.h>
#include <ft_printf.h>  //my printf library

//need these three for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	else if (status != 0)
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

char			*read_all(int fd)
{
	char	buff[BUFF_SIZE + 1];
	int		ret;
	t_lstr	*str;

	str = NULL;
	while ((ret = read(fd, &buff, BUFF_SIZE)))
	{
		buff[ret] = '\0';
		ft_lstr_add(&str, strdup(buff), false);
	}
	return (ft_lstr_finish(&str));
}

char			*get_file_contents(char *file)
{
	int		fd;

	if (!file)
		return (NULL);
	if ((fd = open(file, O_RDONLY)) == -1)
		return (NULL);
	return (read_all(fd));
}

void			test(_Bool last, char const *name,
									int (*test)(void), char *target_output)
{
	static int	total;
	static int	failed;
	int		status;
	pid_t	f;
	int		pipefd[2];
	char	*output;


	total++;
	target_output = get_file_contents(target_output);
	if (target_output == NULL)
		target_output = "\0";
	ft_printf(" > %s:", name);

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
	output = read_all(pipefd[0]);
	if (WIFSIGNALED(status))
		handle_signal(WTERMSIG(status));
	else if (WIFEXITED(status))
		handle_exit(&failed, (int)(char)WEXITSTATUS(status));
	else
		ft_putstr("unknown wait result");
	ft_putchar('\n');
	if (strcmp(target_output, output) != 0)
	{
		ft_putstr("output mismatch:\n");
		ft_printf("actual output:\n->%s<-\n", output);
		ft_printf("target output:\n->%s<-\n", target_output);
	}
	if (last)
	{
		print_result(total, failed);
		total = 0;
		failed = 0;
	}
	close(pipefd[0]);
}
