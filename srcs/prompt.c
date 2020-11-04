/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	char c = '\0';
	printf("[Minishell]>");
	while(c != EOF) {
		c = getchar();
		if(c == '\n')
			printf("[Minishell]>");
	}
	printf("\n");
	return 0;
}
*/

#include "libft.h"
#include <stdio.h>
#include <signal.h>

typedef void (*sighandler_t)(int);

void handle_signal(int signo)
{
	ft_putstr_fd("\n[minishell]>", 1);
}

int main(int argc, char *argv[], char *envp[])
{
	char	*line;
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	int i = 1;	
	while(i)
	{
		ft_putstr_fd("[minishell]>", 1);
		i = get_next_line(0, &line);
		//parse
		if (i)
			ft_putstr_fd("line :", 1);
		ft_putendl_fd(line, 1);
		//exec
		free(line);
	}
	write(1, "\n", 1);
	return 0;
}
