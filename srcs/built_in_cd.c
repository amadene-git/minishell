#include "../includes/minishell.h"

void	refresh_pwd(char *newpwd, t_dlist *envlist, int fd)
{
	char	**tab;
	t_dlist *pwd = dlist_chr(envlist, "PWD");
	t_dlist *oldpwd = dlist_chr(envlist, "OLDPWD");

	if (!pwd || !oldpwd)
	{
		printf("refresh_pwd erreur !\n");
		return ;
	}
	if (oldpwd && oldpwd->data->value)
	{
		free(oldpwd->data->value);
		oldpwd->data->value = NULL;
	}
	if (pwd)
	{
		oldpwd->data->value = pwd->data->value;
		pwd->data->value = newpwd;
	}
	//printf("$PWD=%s\n", elem->data->value);
}

int		built_in_cd(int ac, char **av, t_dlist *envlist, int fd)//cd -> $HOME
{
	char	buff[3200];

	if (ac > 2)
		return(-1);
	if (ac == 1)
	{
		if (!dlist_chr(envlist, "HOME"))
		{
			dprintf(2, "minishell: cd: HOME not set\n");
			return (-1);
		}
		if (!dlist_chr(envlist, "HOME")->data->value)
		{
			dprintf(2, "minishell: cd: HOME not set\n");
			return (-1);
		}
		else if (chdir(dlist_chr(envlist, "HOME")) == -1)
		{
			dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
			return (-1);
		}
		
	}
	else if (chdir(av[1]) == -1)
	{
		dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (-1);
	}
	if (!getcwd(&buff[0], 31999))
	{
		dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (-1);
	}
	refresh_pwd(strdup(&buff[0]), envlist, fd);
	return (0);
}
