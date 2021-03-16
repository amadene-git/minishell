/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 22:41:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 23:22:48 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			shlvl_plusplus(t_dlist *shlvl)
{
	long int nb;

	nb = (unsigned)ft_atoi(shlvl->data->value);
	nb = (nb >= 0) ? nb + 1 : 0;
	free(shlvl->data->value);
	shlvl->data->value = NULL;
	shlvl->data->value = ft_itoa(nb);
}

static t_dlist	*init_pwd(t_dlist *envlist)
{
	t_dlist	*elem;
	char	*buff;
	char	*tmp;

	elem = NULL;
	if (!(elem = dlist_chr(envlist, "PWD")))
	{
		buff = ft_calloc(PATHSIZE + 1, sizeof(char));
		if (!getcwd(buff, PATHSIZE))
		{
			ft_dprintf(2, "minishell: : getcwd: %s\n", strerror(errno));
			return (NULL);
		}
		else
		{
			tmp = ft_strjoindoublefree(ft_strdup("PWD="), ft_strdup(buff));
			elem = insert_var(envlist, create_var((const char*)tmp));
			free(tmp);
		}
		free(buff);
	}
	return (elem);
}

t_dlist			*init_env(const char **env)
{
	t_dlist	*envlist;
	t_dlist	*elem;
	char	*tmp;

	envlist = get_envlist_from_env(env);
	elem = init_pwd(envlist);
	if (!(elem = dlist_chr(envlist, "SHLVL")))
	{
		tmp = ft_strjoindoublefree(ft_strdup("SHLVL="), ft_strdup("1"));
		elem = insert_var(envlist, create_var((const char*)tmp));
		free(tmp);
	}
	else
	{
		shlvl_plusplus(elem);
		clean_spaces(elem);
	}
	if (!(elem = dlist_chr(envlist, "_")))
	{
		tmp = ft_strjoindoublefree(ft_strdup("_="), ft_strdup("?"));
		elem = insert_var(envlist, create_var((const char*)tmp));
		free(tmp);
	}
	return (envlist);
}

static int		get_envlist_elem(const char **env, t_dlist **elem1,
								t_dlist **elem2)
{
	int	i;

	i = 1;
	while (env[i])
	{
		if (!(*elem2 = dlist_create_elem(create_var(env[i++]))))
			return (0);
		(*elem2)->prev = *elem1;
		(*elem1)->next = *elem2;
		if (env[i])
		{
			if (!(*elem1 = dlist_create_elem(create_var(env[i++]))))
				return (0);
			(*elem1)->prev = *elem2;
			(*elem2)->next = *elem1;
		}
	}
	return (i);
}

t_dlist			*get_envlist_from_env(const char **env)
{
	int		i;
	t_dlist *begin;
	t_dlist *elem1;
	t_dlist	*elem2;

	if (!env[0] || !env)
		return (NULL);
	begin = dlist_create_elem(create_var(env[0]));
	elem1 = begin;
	elem2 = NULL;
	i = get_envlist_elem(env, &elem1, &elem2);
	if (!i)
		return (NULL);
	if (!(i % 2))
	{
		begin->prev = elem2;
		elem2->next = begin;
	}
	else
	{
		begin->prev = elem1;
		elem1->next = begin;
	}
	return (begin);
}
