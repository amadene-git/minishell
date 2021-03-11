#include "../includes/minishell.h"

void	refresh_pwd(char *newpwd, t_dlist *envlist)
{
	t_dlist *pwd;
	t_dlist *oldpwd = dlist_chr(envlist, "OLDPWD");

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

int		built_in_cd(int ac, char **av, t_dlist *envlist)//cd -> $HOME
{
	char	*buff;
	t_dlist	*home;


	//printf ("cd !\n");
	if (ac == 1)
	{
		home = dlist_chr(envlist, "HOME");
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
	refresh_pwd(strdup(buff), envlist);
	free(buff);
	return (0);
}
