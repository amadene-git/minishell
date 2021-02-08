#include "../includes/minishell.h"

void	get_absolute_path(char **cmd, t_dlist *envlist)
{
	char			*path = NULL;
	char			*strdir = NULL;
	DIR				*dir;
	struct dirent	*sd;

	if (dlist_chr(envlist, "PATH"))
		path = dlist_chr(envlist, "PATH")->data->value;
	else
		return;
	if (cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0)
		while (path)
		{
			if (ft_strchr(path, ':'))
			{
				strdir = ft_strndup(path, ft_strchr(path, ':') - path);
				path = ft_strchr(path, ':') + 1;
			}
			else
			{
				strdir = ft_strdup(path);
				path = NULL;
			}
			if ((dir = opendir(strdir)) == NULL)
				continue;
			while ((sd = readdir(dir)) != NULL)
				if (!ft_strcmp(sd->d_name, cmd[0]))
				{
					free(cmd[0]);
					strdir = ft_strjoindoublefree(strdir, ft_strdup("/"));
					cmd[0] = ft_strjoindoublefree(strdir, ft_strdup(sd->d_name));
					return;
				}
			closedir(dir);
			free(strdir);
		}
}

int		is_builtin(char	*cmd)
{
	if (!ft_strcmp("echo", cmd) ||\
			!ft_strcmp("pwd", cmd) ||\
			!ft_strcmp("cd", cmd) ||\
			!ft_strcmp("env", cmd) ||\
			!ft_strcmp("export", cmd) ||\
			!ft_strcmp("unset", cmd))
			//!ft_strcmp("exit", cmd))
		return (1);
	return (0);
}

void	exec_built_in(int ac, char **cmd, t_dlist *envlist, int fd)
{
	if (!ft_strcmp("echo", cmd[0]))
	{
		built_in_echo(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("cd", cmd[0]))
	{
		built_in_cd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("pwd", cmd[0]))
	{
		built_in_pwd(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("env", cmd[0]))
	{
		built_in_env(ac , cmd, envlist, fd);
	}
	else if (!ft_strcmp("export", cmd[0]))
	{
		built_in_export(ac, cmd, envlist, fd);
	}
	else if (!ft_strcmp("unset", cmd[0]))
	{
		built_in_unset(ac, cmd, envlist, fd);
	}
}

int	exec_bin(char **cmd, char **env, t_dlist *envlist)
{
	get_absolute_path(cmd, envlist);
	if (execve(cmd[0], cmd, env) == -1)
	{
		dprintf(2, "minishell: %s: command not found\n", cmd[0]);
		return (-1);
	}
	return (0);
}
