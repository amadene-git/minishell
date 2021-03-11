#include "../includes/minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal(int signo)
{
	if (signo == SIGINT)
		ft_putstr_fd("\n[minishell]>", 1);
	if (signo == SIGQUIT)
		ft_dprintf(2, "\b\b  \b\b");
}

void stock_env_status(int status, t_dlist *envlist)
{
	t_var	*var;
	t_dlist	*elem;

	var = (t_var*)malloc(sizeof(t_var));
	var->name = ft_strdup("?");
	var->value = ft_strdup(ft_itoa(status));
	if (!(elem = dlist_chr(envlist, var->name)))
	{
		free(elem->data->value);
		elem->data->value = var->value;
	}
	// else if (var->value)
	// {
	// 	free_var(elem->data);
	// 	elem->data = var;
	// }

}

int exec_cmd(t_cmd *cmd)//copier envlist dans env
{
	int		status;
	pid_t	pid = 0;

	status = 0;
	cmd->pid = fork();
	if (cmd->pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	if (cmd->pid > 0)
	{
		pid = cmd->pid;
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
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
		else if (cmd->bin && !is_builtin(cmd->bin) && (status = exec_bin(cmd)))// copier envlist
		{
			exit(status);
		}
		else
		{
			exit(exec_built_in(cmd));
		}
	}
	if (cmd->prev && cmd->prev->fdpipe)
	{
		close(cmd->prev->fdpipe[0]);
		close(cmd->prev->fdpipe[1]);
	}
	waitpid(cmd->pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == 3)
		ft_dprintf(2, "Quitter (core dumped)\n");
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	status = WEXITSTATUS(status);
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

void	refresh_last_cmd(t_dlist *envlist, char *last_cmd)
{
	t_dlist *last;

	if (!(last = dlist_chr(envlist, "_")))
		last = insert_var(envlist, create_var("_="));
	if (last->data->value)
		free(last->data->value);
	last->data->value = last_cmd;
}

int main(int ac,const char **av, const char	**env)
{
	signal(SIGINT, SIG_IGN);//gestion du crtl+C
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);

	char	*line;
	int		gnl = 1;
	int		k;
	t_tok	**tok_lex;
	t_dlist	*envlist = init_env(env);
	t_cmd 	*cmd;
	t_cmd	*tmp = NULL;
	int		fd[2];
	int		pipe_flag;
	int		status;

	status = 0;
	while(gnl)
	{
		if (ac == 1)
		{
			ft_dprintf(1, "[minishell]>");
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
			//l = dlist_chr(envlist, "PATH");
			//dprintf(2, "lst: type: %s, value: %s\n", l->data->name, l->data->value);
			//while (tok_lex[++k]->type != CHR_END)
			// 	printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			//printf("tok %d type:%d value:%s|\n", k, tok_lex[k]->type, (char*)(tok_lex[k]->value));
			if (!has_errors(tok_lex))
				while ((*tok_lex)->type != CHR_END)
				{
					while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP\
					|| (*tok_lex)->type == CHR_PI || (*tok_lex)->type == CHR_PV)
					{
						tok_lex++;
					}
					cmd = (t_cmd*)malloc(sizeof(t_cmd));
					cmd->envlist = envlist;
					//cmd->env = env;
					cmd->fdin = -1;
					cmd->fdout = -1;
					cmd->fdpipe = NULL;
					cmd->prev = (tmp) ? tmp : NULL;
					cmd->next = NULL;
					cmd->tok_arg = NULL;
					cmd->tok_lst = NULL;
					
					if (tmp)
						tmp->next = cmd;
					if (has_pipe(tok_lex) == 1)
					{
						cmd->fdpipe = (int*)malloc(sizeof(int) * 2);
						if (pipe(cmd->fdpipe) == -1)
							ft_dprintf(2, "erreur main:%s\n", strerror(errno));
					}
					//printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
					cmd->env = get_env_from_envlist(envlist, envlist, 0);
					tok_lex = get_tok_arg(tok_lex, cmd);
					//printf("get_tok_arg\n");
					/*printf("get ac av, ac=%d\n", cmd->ac);
					for (int i  = 0; i <= cmd->ac; i++)
					{
						if (cmd->av)
							printf("av[%d]->%s\n",i, cmd->av[i]);
					}*/
					//tok_lex = get_cmd_new(tok_lex, cmd);
					/*t_tok	*t = cmd->tok_arg;
					while (t)
					{
						dprintf(2, "tok : %s->%d \n", t->value, t->type);
						t = t->next;
					}*/
					enable_redirect(cmd);
					/*t = cmd->tok_arg;
					while (t)
					{
						dprintf(2, "tok : %s->%d \n", t->value, t->type);
						t = t->next;
					}*/
					get_ac_av(cmd->tok_arg, cmd, 0);
					if (cmd->tok_arg && cmd->av && cmd->av[0])
					{

						cmd->bin = ft_strdup(cmd->av[0]);
						prepare_cmd(cmd);
						// printf ("ac :%d\n", cmd->ac);
						// for (int k = 0; cmd->av[k]; k++)
						// 	dprintf(2, "av[%d]:\"%s\"\n", k, cmd->av[k]);
						// 	dprintf(2, "stdout:\n");
				//		printf ("cmd->bin %s\ncmd->av %s\n", cmd->bin, cmd->av[0]);
						status = exec_no_fork(cmd);
					//	if (status == 255)
					//		break;
						if (status == 0)
							status = exec_cmd(cmd);
						//	printf ("status = %d\n", status);
							//printf("cmd->ac = %d last = %s\n", cmd->ac, cmd->av[cmd->ac - 1]);
						// printf ("currtok:%s->%d\n", (char*)(*tok_lex)->value, (*tok_lex)->type);
						//free(cmd->env);
					}
					k = 0;
					tmp = cmd;
					//stock_env_status(status, envlist);
				}
			else 
			{
				status = 2;
			}
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	if (ac == 1)
		ft_dprintf(1, "exit\n");
	exit (status);
	//return (3);
}
