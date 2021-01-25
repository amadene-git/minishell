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

char	*put_var_env(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;
	char	*name;
	char	*var;
	t_dlist *elem;

	i = 0;
	while (str[i])
	{
		j = i;
		if (str[i] == '\\' && str[i + 1])
			i += 2;
		else if (str[j++] == '$')
		{
			while (ft_isalpha(str[j]))
				j++;
			name = ft_strndup(str + i + 1, j - i);
			if (elem = dlist_chr(envlist, name))
				var = elem->data->value;
			str = insert_string(str, ft_strdup(dlist_chr(envlist, name)->data->value), i, j );
			i += ft_strlen(name);
			free(name);
		}
		else
			i++;
	}
	return (str);

}

char	*get_backslash(char *str)
{
	int 	i;
	char	c[2];

	c[1] = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i] == 't')
				c[0] = '\t';
			else if (str[i] == 'n')
				c[0] = '\n';
			else
				c[0] = str[i];
			str = insert_string(str, ft_strdup(&c[0]), i - 1, i--);
		}
		else
			i++;
	}
	return (str);

}

char	*get_dquotes(const char *str, int *i, t_dlist *envlist)// echo "\ " -> \     echo "\\" -> \     echo "\\\\" -> "\\"
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
		return (put_var_env(ft_strndup((char*)&str[j + 1], (*i)++ - j - 1), envlist));
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

char	*get_expr(const char *str, int *i, t_dlist *envlist)//echo \ ('\' + ' ')   ->        ; echo \\ ->  "\\"
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
	
	return (put_var_env(ft_strndup((char*)&str[j], *i - j), envlist));
}

char	*get_next_word(const char *str, int *i, t_dlist *envlist)//bug concatenation "abc""def"
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
				s2 = get_dquotes(str, i, envlist);
			}
			else if (str[*i])//tout le reste
			{
				s2 = get_expr(str, i, envlist);
			}
			s1 = ft_strjoindoublefree(s1, s2);
		}	
	return (s1); 
}

char	**split_cmdline(const char *str, int *i, int n, t_dlist *envlist)
{
	char	*s;
	char	**tab = NULL;

	s = get_next_word(str, i, envlist);
	if (s)
		tab = split_cmdline(str, i, n + 1, envlist);
	if (!s)
		tab = (char**)malloc(sizeof(char*) * (n + 1));
	
    //printf ("s[%d] %s\n", n, s);
	tab[n] = s;
	//printf ("tab[%d]%p-%p->%p %s\n", n, tab, tab[n], s, tab[n]);
	return (tab);
}

