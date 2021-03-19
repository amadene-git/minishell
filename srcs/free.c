/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 16:05:33 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/19 17:34:51 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_lexer(t_tok **tok_lex, int lvl)
{
	if (!tok_lex)
		return ;
	if (tok_lex[lvl + 1] && tok_lex[lvl + 1]->type < CHR_END)
		free_lexer(tok_lex, lvl + 1);
	else
	{
		if (tok_lex[lvl + 1])
		{
			if (tok_lex[lvl + 1]->value)
				free(tok_lex[lvl + 1]->value);
			free(tok_lex[lvl + 1]);
		}
		if (tok_lex[lvl]->value)
			free(tok_lex[lvl]->value);
		free(tok_lex[lvl]);
		if (!lvl)
			free(tok_lex);
		return ;
	}
	if (tok_lex[lvl]->value)
		free(tok_lex[lvl]->value);
	free(tok_lex[lvl]);
	if (!lvl)
		free(tok_lex);
}

void	free_av(char **av, int lvl)
{
	if (!av)
		return ;
	if (av[lvl] && av[lvl + 1])
		free_av(av, lvl + 1);
	free(av[lvl]);
	if (!lvl)
		free(av);
}

void	free_cmd_lst(t_cmd *cmd)
{
	t_cmd *tmp;

	if (!cmd)
		return ;
	while (cmd->next)
		cmd = cmd->next;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->prev;
		free(tmp);
	}
}

void	ses(t_dlist *envlist, char *lastcmd)
{
	t_var	*var;
	t_dlist	*elem;

	var = (t_var*)malloc(sizeof(t_var));
	var->name = ft_strdup("_");
	var->value = ft_strdup(lastcmd);
	if ((elem = dlist_chr(envlist, var->name)))
	{
		free(elem->data->value);
		elem->data->value = ft_strdup(var->value);
		free_var(var);
	}
	else
		envlist = insert_var(envlist, var);
}

void	free_process(t_minishell *shell)
{
	free_av(shell->cmd->env, 0);
	free_av(shell->cmd->av, 0);
	free_tok_arg(shell->cmd->tok_arg);
	free(shell->cmd->bin);
}
