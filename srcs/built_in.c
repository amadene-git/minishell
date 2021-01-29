#include "minishell.h"

int		built_in_echo(int ac, char **av, t_dlist *envlist, int fd)
{
	int	i;
	int	n;
	int	j;

	n = 0;
	i = 0;
	while (++i < ac)
	{
		j = 0;
		if (av[i][0] == '-' && i == n + 1)
		{
			j++;
			while (av[i][j] && av[i][j] == 'n')// ou *av[i] == 'e'
				j++;
			if (!av[i][j])
				n = i;
			else
				ft_putstr_fd(av[i], fd);
		}
		else
			ft_putstr_fd(av[i], fd);
		if (i < ac - 1 && i > n)
			ft_putchar_fd(' ', fd);
	}
	if (!n)
		ft_putchar_fd('\n', fd);
	return (0);
}

void	refresh_pwd(const char *pwd, const char *old, t_dlist *envlist, int fd)
{
	char	**tab;

	if (!(tab = (char**)malloc(sizeof(char*) * 3)))
		return;

	tab[0] = ft_strdup("export");
	tab[1] = ft_strjoin("PWD=", pwd);
	tab[2] = ft_strjoin("OLDPWD=", old);
	built_in_export(3, tab, envlist, fd);

	free(tab[0]);
	free(tab[1]);
	free(tab[2]);
	free(tab);
	t_dlist *elem = dlist_chr(envlist, "PWD");

	//printf("$PWD=%s\n", elem->data->value);
}

int		built_in_cd(int ac, char **av, t_dlist *envlist, int fd)//cd -> $HOME
{
	char	buff[3200];

	if (ac != 2)
		return(-1);
	//if (!path)
	//	printf("path -> $HOME");
	if (chdir(av[1]) == -1)
	{
		dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (-1);
	}
	if (!getcwd(&buff[0], 31999))
	{
		dprintf(2, "minishell: cd: %s: %s\n", av[1], strerror(errno));
		return (-1);
	}
	refresh_pwd(&buff[0], dlist_chr(envlist, "PWD")->data->value, envlist, fd);
	return (0);
}

int		built_in_pwd(int ac, char **av, t_dlist *envlist, int fd)
{
	char	buf[3200];

	if (ac != 1)
		return (-1);
	if (!getcwd(&buf[0], 31999))
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}	
	ft_putendl_fd(&buf[0], fd);
	return (0);
}

