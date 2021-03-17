char	*get_st(char *str)
{
	int	i;

	if (*str != '\'')
		return (NULL);
	i = 0;
	while (str[++i]);
	return (ft_strndup(str + 1, i - 2));
}

char	*str_clean_whitespaces(const char *str)
{
	int 	i;
	char	**tab;
	char	*ret;

	tab = ft_split(str, ' ');
	i = 0;
	if (tab && tab[i])
		ret = ft_strdup(tab[i]);
	i = 1;
	while (tab[i])
	{
		ret = ft_strjoindoublefree(ret, ft_strdup(" "));
		ret = ft_strjoindoublefree(ret, tab[i]);
		i++;
	}
	return (ret);
}

void	get_var(char *str, int *i, int *j, t_dlist *envlist)
{
	char	*ptr;
	t_dlist	*elem;

	(*j) = (*i) + 1;
    while (ft_isalpha(str[(*j)]) || ft_isdigit(str[(*j)]) || str[(*j)] == '_' || str[*j] == '?')
		(*j)++;
	ptr = ft_strndup(str + (*i) + 1, (*j) - ((*i) + 1));
	if ((elem = dlist_chr(envlist, ptr)) && elem->data->value && elem->data->value[0])
	{
        str = insert_string(str, str_clean_whitespaces(elem->data->value), (*i), (*j));
		(*i) += ft_strlen(elem->data->value) - 1;
	}
	else
	{
          	str = insert_string(str, strdup(""), (*i), (*j));
		(*i)--;
	}
	free(ptr);
}

char	*get_str(char *str, t_dlist *envlist)
{
	int		i;
	int		j;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
			if (str[i + 1] == '"' || str[i + 1] == '$' || str[i + 1] == '\\')
            	str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
			get_var(str, &i, &j, envlist);
		i++;
    }
	return (str);
}


char	*get_word(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;

	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
            str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
        {
			get_var(str, &i, &j, envlist);
		}
		i++;
    }
	return (str);
}

