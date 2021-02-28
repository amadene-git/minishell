#include "../includes/minishell.h"

unsigned int g_get_chr[255] = {
	['!'] = CHR_WORD,
	['#'...'%'] = CHR_WORD,
	['*'...':'] = CHR_WORD,
	['?'...'{'] = CHR_WORD,
	['}'...'~']	= CHR_WORD,
	['='] = CHR_WORD,
	['"'] = CHR_STR,
	['\''] = CHR_ST,
	['\n'] = CHR_SP,
	['\t'] = CHR_SP,
	['\v'] = CHR_SP,
	['\r'] = CHR_SP,
	['\f'] = CHR_SP,
	[' '] = CHR_SP,
	['&'] = CHR_OP,
	['|'] = CHR_PI,
	['>'] = CHR_RE,
	['<'] = CHR_RE,
	[';'] = CHR_PV,
	['\0'] = CHR_END
};

t_tok	**lexer(char *str, int *i, int lvl)
{
	t_tok	**tab;
	char	*value;
	int		type;
	int		j;

//	printf ("*******lexer str:%s i:%d lvl:%d*******\n", str + *i, *i, lvl);
	
//	while (g_token_rules[g_get_token[g_get_chr[' ']]][g_get_chr[str[*i]]])
//		(*i)++;
	j = *i;
	value = NULL;
	type = CHR_END;
	if (g_get_chr[str[*i]] == CHR_STR)
	{
		while (g_get_chr[str[++j]] != CHR_STR && str[j])
			if (str[j] == '\\' && str[j + 1])
				j++;
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
	}
	else if (g_get_chr[str[*i]] == CHR_ST)
	{
		while (g_get_chr[str[++j]] != CHR_ST && str[j]);
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
		
	}
	else if (g_get_chr[str[*i]] > CHR_SP && g_get_chr[str[*i]] < CHR_END)
	{
		while (g_get_chr[str[j]] > CHR_SP && g_get_chr[str[j]] < CHR_END)//tant que différent d'un mot
			j++;
		if (j > *i + 2)//la taille du tok->value ne doit pas dépasser 2
			type = CHR_ERROR;
		else if (g_get_chr[str[*i]] == CHR_OP)
			type = CHR_ERROR;
		else if (g_get_chr[str[*i]] == CHR_PI)
		{
			if (j != *i + 1)
				type = CHR_ERROR;
		}
		else if (g_get_chr[str[*i]] == CHR_RE)
		{
			if (!ft_strcmp("<>", str + *i) || !ft_strcmp("><", str + *i) || !ft_strcmp("<<", str + *i))
				type = CHR_ERROR;
		}
		else if (g_get_chr[str[*i]] == CHR_PV)
		{
			if (j != *i + 1)
				type = CHR_ERROR;
		}
	}
	else
		while (g_get_chr[str[*i]] == g_get_chr[str[j]])
		{
			if (str[j]== '\\' && str[j + 1])
				j++;
			if (str[j])
				j++;
			else
				type = CHR_ERROR;
		}
	if (g_get_chr[str[*i]] != CHR_SP)
		value = ft_strndup(str + *i, j - *i);
	else
		value = NULL;
	if (type != CHR_ERROR)
		type = g_get_chr[str[*i]];
	*i = j;
	if (g_get_chr[str[j]] != CHR_END)
		tab = lexer(str, i, lvl + 1);
	else
	{
		tab = (t_tok**)malloc(sizeof(t_tok*) * (lvl + 2));
		tab[lvl + 1] = create_tok(CHR_END, "newline");
	}	
	tab[lvl] = create_tok(type, value);	
	return (tab);
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
	//	printf("1\n");
		j = i;
		if (str[i] == '\\' && str[i + 1])
			i += 2;
		else if (str[j++] == '$')
		{
			while (ft_isalpha(str[j]) || ft_isdigit(str[j]) || str[j] == '_')
				j++;
	//		printf("2\n");
			name = ft_strndup(str + i + 1, j - i - 1);
			if (dlist_chr(envlist, name))
				str = insert_string(str, ft_strdup(dlist_chr(envlist, name)->data->value), i, j);
			else
				str = insert_string(str, ft_strdup(""), i, j);
			free(name);
		}
		else
			i++;
	}
	return (str);

}

char	*get_st(char *str)
{
	int	i;

	if (*str != '\'')
		return (NULL);
	i = 0;
	while (str[++i]);
	return (ft_strndup(str + 1, i - 2));
}

char	*get_str(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;
	char	*ptr;
	t_dlist	*elem;

	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
			if (str[i + 1] == '"' || str[i + 1] == '$' || str[i + 1] == '\\')
            	str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
        {
            j = i + 1;
            while (ft_isalpha(str[j]) || ft_isdigit(str[j]) || str[j] == '_')
                j++;
            ptr = ft_strndup(str + i + 1, j - (i + 1));
			if ((elem = dlist_chr(envlist, ptr)) && elem->data->value)
			{
            	str = insert_string(str, ft_strdup(elem->data->value), i, j);
				i += ft_strlen(elem->data->value) - 1;
			}
			else
			{
            	str = insert_string(str, strdup(""), i, j);
				i--;
			}
        }
		i++;
    }
	return (str);
}

char	*str_clean_whitespaces(const char *str)
{
	int 	i;
	char	**tab;
	char	*ret;

	tab = ft_split(str, ' ');
	i = 0;
	if (tab && tab[i])
		ret =ft_strdup(tab[i]);
	i = 1;
	while (tab[i])
	{
		ret = ft_strjoindoublefree(ret, ft_strdup(" "));
		ret = ft_strjoindoublefree(ret, tab[i]);
		i++;
	}
	return (ret);
}

char	*get_word(char *str, t_dlist *envlist)
{
	int		i;
	int 	j;
	char	*ptr;
	t_dlist	*elem;

	i = 0;
	while (str[i])
    {
        if (str[i] == '\\')
        {
            str = insert_string(str, strdup(""), i, i + 1);
        }
        else if (str[i] == '$')
        {
            j = i + 1;
            while (ft_isalpha(str[j]) || ft_isdigit(str[j]) || str[j] == '_')
                j++;
            ptr = ft_strndup(str + i + 1, j - (i + 1));
			if ((elem = dlist_chr(envlist, ptr)) && elem->data->value && elem->data->value[0])
			{
            	str = insert_string(str, str_clean_whitespaces(elem->data->value), i, j);
				i += ft_strlen(elem->data->value) - 1;
			}
			else
			{
            	str = insert_string(str, strdup(""), i, j);
				i--;
			}
		}
		i++;
    }
	return (str);
}

t_tok	**get_cmd(t_tok **tok_lex,  t_cmd *cmd, int lvl)
{
	int		i;
	char	*str;
	char	*s1 = NULL;
	char	*s2 = NULL;

	i = 0;
	if (tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_SP)
	{
		while (tok_lex[i]->type == CHR_SP)
			i++;
		while (tok_lex[i]->type >= CHR_WORD && tok_lex[i]->type <= CHR_ST)
		{
			if (tok_lex[i]->type == CHR_ST)
			{
				s2 = ft_strndup(tok_lex[i]->value + 1, ft_strlen(tok_lex[i]->value) - 2);
			}
			else if (tok_lex[i]->type == CHR_STR)
			{
				s2 = get_str(ft_strndup(tok_lex[i]->value + 1, ft_strlen(tok_lex[i]->value) - 2), cmd->envlist);
			}
			else if (tok_lex[i]->type == CHR_WORD)
			{
				s2 = get_word(ft_strdup(tok_lex[i]->value), cmd->envlist);
			}
			s1 = ft_strjoindoublefree(s1, s2);
			i++;
		}
		tok_lex = get_cmd(tok_lex + i, cmd, lvl + 1);
	}
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = (tok_lex[i - 1]->type == CHR_SP) ? lvl - 1 : lvl;
		//cmd->ac = lvl;
		return (tok_lex + i);
	}
	cmd->av[lvl] = s1;
	return (tok_lex);

}
