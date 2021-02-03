#include "../includes/minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal()
{
	ft_putstr_fd("\n[minishell]>", 1);
}

int exec_cmd(t_cmd *cmd, int fd[2], int flag)//copier envlist dans env
{
	if (!ft_strcmp("exit", cmd->av[0]))
	{
		return (0);
	}
	else if (cmd->av[0] && !is_builtin(cmd->av[0]) && exec_bin(cmd->av, (char**)cmd->env, flag, fd))// copier envlist
	{
		return (-1);
	}
	else
		exec_built_in(cmd->ac, cmd->av, cmd->envlist, 1);
	return (0);
}

int	has_pipe(t_tok **tok_lex)
{
	int	i;

	i = 0;
	while (tok_lex[i]->type < CHR_END)
	{
		if (tok_lex[i]->type == CHR_PI)
			return (1);
		i++;
	}
	return (0);
}

int main(int ac,const char **av, const char	**env)
{
	signal(SIGINT, SIG_IGN);//gestion du crtl+C
	signal(SIGINT, handle_signal);

	char	*line;
	int		gnl = 1;
	int		k;
	t_tok	**tok_lex;
	t_dlist	*envlist = dlist_create_from_tab(env);
	t_cmd 	*cmd;
	int		fd[2];
	int		pipe_flag;

	fd[0] = 0;
	fd[1] = 0;
	pipe_flag = 0;
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
			 //k = -1;
			 //while (tok_lex[++k]->type != CHR_END)
			 	//printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			 //printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			if (has_pipe(tok_lex))
				pipe_flag++;
			if (pipe_flag == 1)
				if (pipe(fd) == -1)
					return (-1);
			while ((*tok_lex)->type < CHR_END)
			{
				while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP\
				|| (*tok_lex)->type == CHR_PI)
				{
					if ((*tok_lex)->type == CHR_PI)
						pipe_flag++;
					tok_lex++;
				}
				//printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
				cmd = (t_cmd*)malloc(sizeof(t_cmd));
				cmd->envlist = envlist;
				cmd->env = env;
				tok_lex = get_cmd(tok_lex, cmd, 0);// cmd incremente tok_lex
				// printf ("ac :%d\n", cmd->ac);
				// for (int k = 0; cmd->av[k]; k++)
				// 	printf("av[%d]:\"%s\"\n", k, cmd->av[k]);
				// printf("stdout:\n");
				exec_cmd(cmd, fd, pipe_flag);
				// printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
			}
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	return (0);
}