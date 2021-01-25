#include "minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal()
{
	ft_putstr_fd("\n[minishell]>", 1);
}


int exec_cmd(int ac, char **cmd, const char **env, t_dlist *envlist, int fd)
{
	if (!ft_strcmp("exit", cmd[0]))
	{
		return (0);
	}
	else if (cmd[0] && !is_builtin(cmd[0]) && exec_bin(cmd, (char**)env))
	{
		return (-1);
	}
	else
		exec_built_in(ac, cmd, envlist, 1);
	return (0);

}

int main(int ac, const char **av, const char **env)
{
	char	*line;
	int		i = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);

	int		j = 0;
	int		k = -1;
	char	**tab;
	char	**cmd;

	t_dlist *envlist = dlist_create_from_tab(env);	

	while(i)
	{
//		
		if (ac == 1)
		{
			ft_putstr_fd("[minishell]", 1);
			ft_putchar_fd('>', 1);
			i = get_next_line(0, &line);
		}
		else 
			line = ft_strdup(av[2]);

		if (i)
		{
			j = 0;             
			

			tab = split_cmdline(line, &j, 0, envlist);
			k = -1;
			cmd = tab;
			while (cmd[++k]);
			if (exec_cmd(k, cmd, env, envlist, 1) && ac != 1)
				return (127);
			j = -1;
			while (++j < k)
				free(tab[j]);
			free(tab);	
		}
		if (ac != 1)
			i = 0;
		free(line);
	}
	free_envlist(envlist);
	return 0;
}

