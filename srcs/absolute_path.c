/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:15:43 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/16 15:28:10 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	devide_path(char **path, char **strdir)
{
	if (ft_strchr(*path, ':'))
	{
		*strdir = ft_strndup(*path, ft_strchr(*path, ':') - *path);
		*path = ft_strchr(*path, ':') + 1;
	}
	else
	{
		*strdir = ft_strdup(*path);
		*path = NULL;
	}
}

static int	join_dir(t_cmd *cmd, DIR **dir, char **strdir)
{
	struct dirent	*sd;

	while ((sd = readdir(*dir)) != NULL)
		if (!ft_strcmpci(sd->d_name, cmd->av[0]))
		{
			*strdir = ft_strjoindoublefree(*strdir, ft_strdup("/"));
			if (cmd->bin)
				ft_memdel((void **)&cmd->bin);
			cmd->bin = ft_strjoindoublefree(*strdir,\
						ft_strdup(sd->d_name));
			if (*dir != NULL)
				closedir(*dir);
			return (1);
		}
	return (0);
}

void		get_absolute_path(t_cmd *cmd, t_dlist *envlist)
{
	char			*path;
	char			*strdir;
	DIR				*dir;

	path = NULL;
	strdir = NULL;
	if (dlist_chr(envlist, "PATH"))
		path = dlist_chr(envlist, "PATH")->data->value;
	else
		return ;
	if (cmd->av[0][0] != '/' && ft_strncmp(cmd->av[0], "./", 2) != 0)
		while (path)
		{
			devide_path(&path, &strdir);
			if ((dir = opendir(strdir)) == NULL)
			{
				free(strdir);
				continue;
			}
			if (join_dir(cmd, &dir, &strdir))
				return ;
			closedir(dir);
			free(strdir);
		}
}
