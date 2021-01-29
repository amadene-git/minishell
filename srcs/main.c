#include "minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal()
{
	ft_putstr_fd("\n[minishell]>", 1);
}

int exec_cmd(t_cmd *cmd)//copier envlist dans env
{
	if (!ft_strcmp("exit", cmd->av[0]))
	{
		return (0);
	}
	else if (cmd->av[0] && !is_builtin(cmd->av[0]) && exec_bin(cmd->av, (char**)cmd->env))// copier envlist
	{
		return (-1);
	}
	else
		exec_built_in(cmd->ac, cmd->av, cmd->envlist, 1);
	return (0);

}



int main(int ac,const char **av, const char	**env)
{

	signal(SIGINT, SIG_IGN);//gestion du crtl+C
	signal(SIGINT, handle_signal);

	char	*line;
	int		gnl = 1;
	int k;
	t_tok	**tok_lex;
	t_dlist	*envlist = dlist_create_from_tab(env);
	t_cmd *cmd;


	 k = -1;
	

	
	while(gnl)
	{
		if (ac == 1)
		{
			ft_putstr_fd("[minishell]", 1);
			ft_putchar_fd('>', 1);
			gnl = get_next_line(0, &line);
		}
		else 
			line = ft_strdup(av[2]);

		if (gnl && *line)
		{
			k = 0;
			tok_lex = lexer(line, &k, 0);
			// k = -1;
			// while (tok_lex[++k]->type != CHR_END)
			// 	printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			// printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			while ((*tok_lex)->type < CHR_END)
			{
				while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP)
					tok_lex++;
				// printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);

				cmd = (t_cmd*)malloc(sizeof(t_cmd));
				cmd->envlist = envlist;
				cmd->env = env;
				tok_lex = get_cmd(tok_lex, cmd, 0);// cmd incremente tok_lex
				// printf ("ac :%d\n", cmd->ac);
				// for (int k = 0; cmd->av[k]; k++)
				// 	printf("av[%d]:\"%s\"\n", k, cmd->av[k]);
				// printf("stdout:\n");
				exec_cmd(cmd);
				// printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
			}
			
			
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	

	return (0);
}

