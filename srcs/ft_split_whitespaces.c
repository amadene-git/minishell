#include "../includes/minishell.h"

int		nb_words(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		while (*str && (*str == ' ' || *str ==  '\n' || *str == '\t'))
			str++;
		if (*str)
			i++;
		while (*str && *str != ' ' && *str !=  '\n' && *str != '\t')
			str++;
	}
	return (i);
}

int		word_len(char *str)
{
	int i;

	i = 0;
	while (*str && (*str == ' ' || *str ==  '\n' || *str == '\t'))
			str++;
	while (str[i] && str[i] != ' ' && str[i] !=  '\n' && str[i] != '\t')
			i++;
	return (i);
}

char **ft_split_whitespaces(char *str)
{
	char **tab;
	int i;
	int j;

	i = 0;
	if(!(tab = (char**)malloc((sizeof(char*) * nb_words(str)) + 1)))
		return (NULL);
	while (nb_words(str))
	{
		j = 0;
		while (*str && (*str == ' ' || *str ==  '\n' || *str == '\t'))
			str++;
		if(!(tab[i] = (char*)malloc(sizeof(char) * word_len(str) + 1)))
			return (NULL);
		while (str[j] && str[j] != ' ' && str[j] !=  '\n' && str[j] != '\t')
		{
			tab[i][j] = str[j];
			j++;
		}
		str += j;
		tab[i][j] = 0;
		i++;
	}
	tab[i] = 0;
	return (tab);
}
