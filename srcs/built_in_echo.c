#include "../includes/minishell.h"

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

