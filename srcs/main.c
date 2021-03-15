#include "../includes/minishell.h"

typedef void (*sighandler_t)(int);

void handle_signal(int signo)
{
	if (signo == SIGINT)
		ft_putstr_fd("\n[minishell]>", 1);
	if (signo == SIGQUIT)
		ft_dprintf(2, "\b\b  \b\b");
}

t_dlist *stock_env_status(int status, t_dlist *envlist)
{
	t_var	*var;
	t_dlist	*elem;

	var = (t_var*)malloc(sizeof(t_var));
	var->name = ft_strdup("?");
	var->value = ft_itoa(status);
	if ((elem = dlist_chr(envlist, var->name)))
	{
		free(elem->data->value);
		elem->data->value = ft_strdup(var->value);
		free_var(var);
	}
	else
		envlist = insert_var(envlist, var);
	return (envlist);
}

int exec_cmd(t_cmd *cmd)//copier envlist dans env
{
	int		status;

	status = 0;
	cmd->pid = fork();
	if (cmd->pid == -1)
		ft_putstr_fd(strerror(errno), 2);
	if (cmd->pid > 0)
	{
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
			cmd->fdin != -1 || cmd->fdout != -1 ? exit(1) : 0;
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

void	free_lexer(t_tok **tok_lex, int lvl)
{
	if (tok_lex[lvl + 1] && tok_lex[lvl + 1]->type < CHR_END)
		free_lexer(tok_lex, lvl + 1);
	else
	{
		if (tok_lex[lvl]->value)
			free(tok_lex[lvl]->value);
		free(tok_lex[lvl]);
		return;
	}
	if (tok_lex[lvl]->value)
		free(tok_lex[lvl]->value);
	free(tok_lex[lvl]);
	if (!lvl)
		free(tok_lex);
}

void	free_av(char **av, int lvl)
{
	if (av[lvl + 1])
		free_av(av, lvl + 1);
	free(av[lvl]);
	if (!lvl)
		free(av);
}

int main(int ac,const char **av, const char	**env)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);

	char	*line;
	int		gnl;
	int		k;
	t_tok	**tok_lex;
	t_tok	**save_lex;
	t_dlist	*envlist;
	t_cmd 	*cmd;
	t_cmd	*tmp = NULL;
	int		status;

	status = 0;
	gnl = 1;
	envlist = init_env(env);
	while(gnl)
	{
		line = NULL;
		cmd = NULL;
		if (ac == 1)
		{
			ft_dprintf(1, "[minishell]>");
			gnl = get_next_line(0, &line);
		}
		else if (ac > 2)
			line = ft_strdup(av[2]);
		if (gnl && line && *line)
		{
			k = 0;
			tok_lex = lexer(line, &k, 0);
			save_lex = tok_lex;
			cmd = NULL;
			if (!has_errors(tok_lex))
				while ((*tok_lex)->type != CHR_END)
				{
					while ((*tok_lex)->type == CHR_SP || (*tok_lex)->type == CHR_OP\
					|| (*tok_lex)->type == CHR_PI || (*tok_lex)->type == CHR_PV)
						tok_lex++;
					cmd = (t_cmd*)malloc(sizeof(t_cmd));
					cmd->envlist = envlist;
					cmd->fdin = -1;
					cmd->fdout = -1;
					cmd->fdpipe = NULL;
					cmd->prev = (tmp) ? tmp : NULL;
					cmd->next = NULL;
					cmd->tok_arg = NULL;
					cmd->bin = NULL;
					cmd->tok_lex = save_lex;
					if (tmp)
						tmp->next = cmd;
					cmd->env = get_env_from_envlist(envlist, envlist, 0);
					envlist = stock_env_status(status, envlist);
					tok_lex = get_tok_arg(tok_lex, cmd);
					status = enable_redirect(cmd);
					if (has_pipe(tok_lex) == 1)
					{
						cmd->fdpipe = (int*)malloc(sizeof(int) * 2);
						if (pipe(cmd->fdpipe) == -1)
							ft_dprintf(2, "erreur main:%s\n", strerror(errno));
					}
					get_ac_av(cmd->tok_arg, cmd, 0);
					if (cmd->tok_arg && cmd->av && cmd->av[0] && !status)
					{
						cmd->bin = ft_strdup(cmd->av[0]);
						prepare_cmd(cmd);
						status = exec_no_fork(cmd);
						if (status == 0)
							status = exec_cmd(cmd);
					}
					k = 0;
					tmp = cmd;
					envlist = stock_env_status(status, envlist);
					free_av(cmd->env, 0);
					free_av(cmd->av, 0);
				}
			else 
				status = 2;
			free_lexer(save_lex, 0);
		}
		if (ac != 1)
			gnl = 0;
		free(line);
	}
	if (cmd)
		free_cmd(cmd);
	free_envlist(envlist);
	if (ac == 1)
		ft_dprintf(1, "exit\n");
	exit (status);
}
