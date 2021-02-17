#include "../includes/minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal()
{
	ft_putstr_fd("\n[minishell]>", 1);
}

int exec_cmd(t_cmd *cmd, int fd[2], int flag, t_dlist *envlist)//copier envlist dans env
{
	int		i;
	int		status;
	pid_t	pid = 0;
	char	**args;
	t_tok	*t;

	i = -1;
	status = 0;
	while (cmd->bin[++i])
		cmd->bin[i] = ft_tolower(cmd->bin[i]);
	i = -1;
	/*t = cmd->tok_lst;
	while (t)
	{
		dprintf(2, "tok : %s \n", t->value);
		t = t->next;
	}*/
	cmd->av = to_char_args(cmd->tok_lst);
	// while(cmd->av[++i])
	// 	dprintf(2,"av(%d) %s |\n", i, cmd->av[i]);
	cmd->ac = tok_list_size(cmd->tok_lst);
	//dprintf(2,"%d", cmd->ac);
	cmd->pid = fork();
	if (cmd->pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	if (cmd->pid > 0)
	{
		pid = cmd->pid;
	}
	else if (cmd->pid == 0)
	{
		if (cmd->prev && cmd->prev->fdpipe)
		{
			dup2(cmd->prev->fdpipe[0], STDIN_FILENO);
			close(cmd->prev->fdpipe[1]);
		}
		if (cmd->fdpipe)
		{
			dup2(cmd->fdpipe[1], STDOUT_FILENO);
			close(cmd->fdpipe[0]);
		}
		if (cmd->fdout != -1)
		{
			dup2(cmd->fdout, STDOUT_FILENO);
			close(cmd->fdout);
		}
		if (cmd->fdin != -1)
		{
			dup2(cmd->fdin, STDIN_FILENO);
			close(cmd->fdin);
		}
		if (!ft_strcmp("exit", cmd->bin))
		{
			status = 0;
		}
		else if (cmd->bin && !is_builtin(cmd->bin) && exec_bin(cmd))// copier envlist
		{
			status = -1;
		}
		else
		{
			exec_built_in(cmd);
			exit (0);
		}
	}
	if (cmd->prev && cmd->prev->fdpipe)
	{
		close(cmd->prev->fdpipe[0]);
		close(cmd->prev->fdpipe[1]);
	}
		waitpid(cmd->pid, NULL, 0);
	return (status);
}

int	has_pipe(t_tok **tok_lex)
{
	int	i;

	i = 0;
	while ((tok_lex[i]->type <= CHR_SP || tok_lex[i]->type == CHR_PI))
	{
		if (tok_lex[i]->type == CHR_PI)
			return (1);
		i++;
	}
	return (0);
}

int	check_error(t_tok *actual, t_tok *prev)
{
	if ((actual->type == CHR_PI || actual->type == CHR_OP) && !prev)
		return (0);
	if (actual->type == CHR_RE && (prev
		&& prev->type == CHR_RE))
		return (0);
	if (actual->type == CHR_END && prev
		&& (prev->type == CHR_RE || prev->type == CHR_PI))
		return (0);
	if ((actual->type == CHR_PI || actual->type == CHR_OP)
		&& (prev->type == CHR_PI || prev->type == CHR_OP
		|| prev->type == CHR_RE))
		return (0);
	return (1);
}

int	has_errors(t_tok **tok_lex)
{
	int		i;

	i = -1;
	while (tok_lex[++i] && tok_lex[i]->type <= CHR_END)
	{
		if ((i == 0 && !check_error(tok_lex[i], NULL)) || !check_error(tok_lex[i], tok_lex[i - 1]))
		{
			dprintf(2, "minishell: syntax error near unexpected token `%s'\n", tok_lex[i]->value);
			return (1);
		}
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
	t_dlist	*envlist = init_env(env);
	t_cmd 	*cmd;
	t_cmd	*tmp = NULL;
	int		fd[2];
	int		pipe_flag;
	t_tok	*t;

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
			fd[0] = 0;
			fd[1] = 0;
			pipe_flag = 0;
			k = 0;
			tok_lex = lexer(line, &k, 0);
			k = -1;
			//while (tok_lex[++k]->type != CHR_END)
			//	printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			//printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			if (!has_errors(tok_lex))
				while ((*tok_lex)->type != CHR_END)
				{
					while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP\
					|| (*tok_lex)->type == CHR_PI)
					{
						tok_lex++;
					}
					cmd = (t_cmd*)malloc(sizeof(t_cmd));
					cmd->envlist = envlist;
					cmd->env = env;
					cmd->fdin = -1;
					cmd->fdout = -1;
					cmd->fdpipe = NULL;
					cmd->prev = (tmp) ? tmp : NULL;
					cmd->next = NULL;
					if (tmp)
						tmp->next = cmd;
					if (has_pipe(tok_lex) == 1)
					{
						cmd->fdpipe = (int*)malloc(sizeof(int) * 2);
						if (pipe(cmd->fdpipe) == -1)
							dprintf(2, "erreur main:%s\n", strerror(errno));
					}
					//printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
					//cmd->env = get_env_from_envlist(envlist, envlist, 0);
					//tok_lex = get_cmd(tok_lex, cmd, 0);// cmd incremente tok_lex
					tok_lex = get_cmd_new(tok_lex, cmd);
					/*t = cmd->tok_lst;
					while (t)
					{
						dprintf(2, "tok : %s \n", t->value);
						t = t->next;
					}*/
					enable_redirect(cmd);
					if (cmd->tok_lst)
					{
						cmd->bin = cmd->tok_lst->value;
						//printf ("ac :%d\n", cmd->ac);
						//for (int k = 0; cmd->av[k]; k++)
							//dprintf(2, "av[%d]:\"%s\"\n", k, cmd->av[k]);
							//dprintf(2, "stdout:\n");
						exec_cmd(cmd, fd, pipe_flag, envlist);
						// printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
						//free(cmd->env);
					}
					tmp = cmd;
				}
			else if (ac != 1)
				return (2);
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	return (0);
}
