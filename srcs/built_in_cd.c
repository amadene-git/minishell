/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 16:55:22 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 17:01:27 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		refresh_pwd(char *newpwd, t_dlist *envlist)
{
	t_dlist *pwd;
	t_dlist *oldpwd;

	oldpwd = dlist_chr(envlist, "OLDPWD");
	if (!(pwd = dlist_chr(envlist, "PWD")))
		pwd = insert_var(envlist, create_var("PWD="));
	if (oldpwd && oldpwd->data->value)
	{
		free(oldpwd->data->value);
		oldpwd->data->value = NULL;
	}
	if (pwd->data->value)
	{
		if (!(oldpwd = dlist_chr(envlist, "OLDPWD")))
			oldpwd = insert_var(envlist, create_var("OLDPWD="));
		oldpwd->data->value = pwd->data->value;
	}
	pwd->data->value = newpwd;
}

static int	handle_home_errors(t_dlist *home, char **av)
{
	if (home && home->data->value)
	{
		if (chdir(home->data->value) == -1)
		{
			ft_dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
			return (1);
		}
	}
	else
	{
		ft_dprintf(2, "minishell: cd: HOME not set\n");
		return (1);
	}
	return (0);
}

int			built_in_cd(int ac, char **av, t_dlist *envlist)
{
	char	*buff;
	t_dlist	*home;

	if (ac == 1)
	{
		home = dlist_chr(envlist, "HOME");
		if (handle_home_errors(home, av))
			return (1);
	}
	else if (ac > 1 && !av[1][0])
		;
	else if (ac > 1 && (chdir(av[1]) == -1))
	{
		ft_dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (1);
	}
	buff = (char*)ft_calloc(PATHSIZE + 1, sizeof(char));
	if (!getcwd(buff, PATHSIZE))
	{
		ft_dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (1);
	}
	refresh_pwd(ft_strdup(buff), envlist);
	free(buff);
	return (0);
}
