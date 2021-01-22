#include "minishell.h"

char	*get_quotes(const char *str, int *i)
{
	int j;

	j = (*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
		return (ft_strndup((char*)&str[j + 1], (*i)++ - j - 1));
	printf("||||||||||||||error quote||||||||||||||||||\n");
	return (NULL);
}

char	*get_dquotes(const char *str, int *i)// echo "\ " -> \     echo "\\" -> \     echo "\\\\" -> "\\"
{
	int j;

	j = (*i)++;
	while (str[*i] && str[*i] != '"')\
	{
		if (str[*i] == '\\' && str[*i + 1])
			(*i) += 2;
		(*i)++;
	}
	//traduire en expression régulière
	if (str[*i] == '"')
		return (ft_strndup((char*)&str[j + 1], (*i)++ - j - 1));
	printf("||||||||||||||||||error dquote||||||||||||||||||||||||\n");
	return (NULL);
	
}

int		is_sep_expr(char c)
{
	
	if (c == '>' \
	|| c == '|' \
	|| c == ';' \
	|| c == '<' )
		return (1);
	return (0);
}

char	*get_expr(const char *str, int *i)//echo \ ('\' + ' ')   ->        ; echo \\ ->  "\\"
{
	int j;

	if (!str[*i] || str[*i] == '\'' || str[*i] == '"')
		return (NULL);
	j = *i;
	while (str[*i] && str[*i] != ' ' && !is_sep_expr(str[*i]) && str[*i] != '\'' && str[*i] != '"')
	{
		if (str[*i] == '\\' && str[*i + 1])
			(*i) += 2;
		(*i)++;
	}
	// traduire regex
	return (ft_strndup((char*)&str[j], *i - j));
}

char	*get_next_word(const char *str, int *i)//bug concatenation "abc""def"
{
	char	*s1 = NULL;
	char	*s2 = NULL;

	if (!str)
		return (NULL);
	if (str[*i] == ' ')//saute les espaces
		while (str[*i] && str[*i] == ' ')
			(*i)++;
	if (str[*i] == '>' || str[*i] == '|' || str[*i] == ';' || str[*i] == '<')//check ; > |...
		if (str[*i] == '>' && str[*i + 1] == '>')
		{
			*i += 2;
			return (ft_strdup(">>"));
		}
		else
			return (ft_strndup((char*)&str[(*i)++], 1));
	else
		while (str[*i] && str[*i] != ' ' && !is_sep_expr(str[*i]))
		{
			if (str[*i] == '\'')//rentre dans des simples quotes
			{
				s2 = get_quotes(str, i);
			}
			else if (str[*i] == '\"')//rentre dans des doubles quotes no fait
			{
				s2 = get_dquotes(str, i);
			}
			else if (str[*i])//tout le reste
			{
				s2 = get_expr(str, i);
			}
			s1 = ft_strjoindoublefree(s1, s2);
		}	
	return (s1); 
}

char	**split_cmdline(const char *str, int *i, int n)
{
	char	*s;
	char	**tab = NULL;

	s = get_next_word(str, i);
	if (s)
		tab = split_cmdline(str, i, n + 1);
	if (!s)
		tab = (char**)malloc(sizeof(char*) * (n + 1));
	
    //printf ("s[%d] %s\n", n, s);
	tab[n] = s;
	//printf ("tab[%d]%p-%p->%p %s\n", n, tab, tab[n], s, tab[n]);
	return (tab);
}

