/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 16:07:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/18 11:14:49 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_signal(int signo)
{
	if (signo == SIGINT)
		ft_putstr_fd("\n[minishell]>", 1);
	if (signo == SIGQUIT)
		ft_dprintf(2, "\b\b  \b\b");
}

t_dlist	*stock_env_status(int status, t_dlist *envlist)
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

int		has_pipe(t_tok **tok_lex)
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

void	init_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
}
