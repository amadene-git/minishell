#include "../minishell.h"

void	built_in_cd(const char *path)//cd -> $HOME
{
	//if (!path)
	//	printf("path -> $HOME");
	if ((chdir(path)) == -1)
		ft_putendl_fd(strerror(errno), 2);
}

int main(int ac, char **av)
{
	char *line;
	while (1)
	{
		system("pwd");
		printf("cd :");
		get_next_line(0, &line);
		built_in_cd((const char*)line);
	}
}

