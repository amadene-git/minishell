#include "../includes/minishell.h"

char	*ft_echo(char **av)
{
	int i;

	i = 0;
	if (av[0][0] == '-')
		if (!strcmp(av[0], "-n"))
			i = 1;
		else
		{
			ft_putstr("option invalide");
			return (NULL);		
		}
	while (av[i])
	{
		ft_putstr(av[i]);
		i++;
		if (av[i])
			ft_putchar(' ');
	}
	if (strcmp(av[0], "-n"))
		ft_putchar('\n');
}

int main(int ac, char **av, char ** envp)
{
	if (ac == 1)
	{
		ft_putchar('\n');
		return (0);
	}
	ft_echo(av + 1);
	return (0);
}
