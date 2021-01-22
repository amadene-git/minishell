#include "minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal()
{
	ft_putstr_fd("\n[minishell]>", 1);
}


int main(int ac, const char **av, const char **env)
{
	char	*line;
	int		i = 1;
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);

	(void)ac;
	(void)av;	
	int		j = 0;
	int		k = -1;
	char	**tab;
	
	t_dlist *envlist = dlist_create_from_tab(env);	

	while(i)
	{
		ft_putstr_fd("[minishell]", 1);
		ft_putchar_fd('>', 1);
		i = get_next_line(0, &line);
		

		if (i && *line)
		{
			j = 0;
			printf ("******************PARSING************\n");//
             
			

			tab = split_cmdline(line, &j, 0);
			printf ("tab->%p\n", tab);//
			k = -1;
			while (tab[++k])
				printf("%d.%s->%p\n", k, tab[k], tab[k]);//
			printf("***********************EXEC***************\n");//
			if (!tab[0])
				ft_putstr_fd("Command not found\n", 1);
			else if (!is_builtin(tab[0]))
			{
				exec_cmd(tab, (char**)env);
			}
			else
				exec_built_in(k, tab, envlist, 1);

			j = -1;
			while (++j < k)
				free(tab[j]);
			free(tab);	
		}
		//free envlist
		free(line);
	}
	write(1, "\n", 1);
	return 0;
}

