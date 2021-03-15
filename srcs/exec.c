#include "../includes/minishell.h"

void	get_absolute_path(t_cmd *cmd, t_dlist *envlist)
{
	char			*path = NULL;
	char			*strdir = NULL;
	DIR				*dir;
	struct dirent	*sd;

	if (dlist_chr(envlist, "PATH"))
		path = dlist_chr(envlist, "PATH")->data->value;
	else
		return;
	if (cmd->av[0][0] != '/' && ft_strncmp(cmd->av[0], "./", 2) != 0)
	{
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
			{
				free(strdir);
				continue;
			}
			while ((sd = readdir(dir)) != NULL)
				if (!ft_strcmpci(sd->d_name, cmd->av[0]))
				{
					strdir = ft_strjoindoublefree(strdir, ft_strdup("/"));
					if (cmd->bin)
						ft_memdel((void **)&cmd->bin);
					cmd->bin = ft_strjoindoublefree(strdir, ft_strdup(sd->d_name));
					if (dir != NULL)
						closedir(dir);
					return;
				}
			closedir(dir);
			free(strdir);
		}
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

int	exec_built_in(t_cmd *cmd)
{
	int status = 0;
	if (!ft_strcmp("echo", cmd->bin))
	{
		status = built_in_echo(cmd->ac, cmd->av, 1);
	}
	else if (!ft_strcmp("pwd", cmd->bin))
	{
		status = built_in_pwd(cmd->ac, cmd->av);
	}
	else if (!ft_strcmp("env", cmd->bin))
	{
		status = built_in_env(cmd);
	}
	else if (!ft_strcmp("export", cmd->bin) && cmd->ac == 1)
	{
		status = built_in_export(cmd->ac, cmd->av, cmd->envlist, 1);
	}
	return(status);
}

int	exec_bin(t_cmd *cmd)
{
	struct stat buff;
	int fd;

	if (lstat(cmd->bin, &buff) != -1)
	{
		if (S_ISLNK(buff.st_mode))
			stat(cmd->bin, &buff);
		if (access(cmd->bin, F_OK))
		{
			ft_dprintf(2, "minishell: %s: No such file or directory\n", cmd->bin);
			return (127);
		}
		else if (buff.st_mode & S_IFREG || S_ISDIR(buff.st_mode))
		{
			if (S_ISDIR(buff.st_mode) && ((cmd->av[0][0]=='.' && cmd->av[0][1] == '/')
				|| cmd->av[0][ft_strlen(cmd->av[0]) - 1] == '/'))
			{
				ft_dprintf(2, "minishell: %s: is a directory\n", cmd->bin);
				return(126);
			}
			else if (buff.st_mode & S_IXUSR)
			{
				fd = open(cmd->bin, O_RDONLY);
				if (fd == -1)
				{
					ft_dprintf(2, "minishell: %s: Permission denied\n", cmd->bin);
					return (126);
				}
				else
					close(fd);
				if (execve(cmd->bin, cmd->av, cmd->env) == -1)
				{
					if (errno == 8 )
						return (0);
					if (errno == 13)
					{
						ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
						return (127);
					}
					return(126);
				}
			}
			else if (ft_strchr(cmd->bin, '/') == NULL)
			{
				ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
				return (127);
			}
			else
			{
				ft_dprintf(2, "minishell: %s: Permission denied\n", cmd->bin);
				return (126);
			}
		}
	}
	else
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd->bin);
		return (127);	
	}	
	return (0);
}

int	exec_no_fork(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (!ft_strcmp("exit", cmd->bin))
	{
		status = exit_cmd(cmd);
	}
	else if (!ft_strcmp("cd", cmd->bin))
	{
		status = built_in_cd(cmd->ac, cmd->av, cmd->envlist);
	}
	else if (!ft_strcmp("unset", cmd->bin))
	{
		status = built_in_unset(cmd);
	}
	else if (!ft_strcmp("export", cmd->bin) && cmd->ac > 1)
	{
		status = built_in_export(cmd->ac, cmd->av, cmd->envlist, 1);
	//	printf ("status %d\n", status);
	}
	return (status);
}
