#include "../minishell.h"

void	built_in_env(char **env, int fd)
{
	int i;
	
	i = -1;
	while (env[++i])
		ft_putendl_fd(env[i], fd);
}

int		main(int ac, char **av, char **env)
{
	built_in_env(env, 1);
	return (0);
}

