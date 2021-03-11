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
	if (g_get_chr[(int)str[*i]] == CHR_STR)
	{
		while (g_get_chr[(int)str[++j]] != CHR_STR && str[j])
			if (str[j] == '\\' && str[j + 1])
				j++;
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
	}
	else if (g_get_chr[(int)str[*i]] == CHR_ST)
	{
		while (g_get_chr[(int)str[++j]] != CHR_ST && str[j]);
		if (str[j])
			j++;
		else
			type = CHR_ERROR;
		
	}
	else if (g_get_chr[(int)str[*i]] > CHR_SP && g_get_chr[(int)str[*i]] < CHR_END)
	{
		while (g_get_chr[(int)str[j]] > CHR_SP && g_get_chr[(int)str[j]] < CHR_END)//tant que différent d'un mot
			j++;
		if (j > *i + 2)//la taille du tok->value ne doit pas dépasser 2
			type = CHR_ERROR;
		else if (g_get_chr[(int)str[*i]] == CHR_OP)
			type = CHR_ERROR;
		else if (g_get_chr[(int)str[*i]] == CHR_PI)
		{
			if (j != *i + 1)
				type = CHR_ERROR;
		}
		else if (g_get_chr[(int)str[*i]] == CHR_RE)
		{
			if (!ft_strcmp("<>", str + *i) || !ft_strcmp("><", str + *i) || !ft_strcmp("<<", str + *i))
				type = CHR_ERROR;
		}
		else if (g_get_chr[(int)str[*i]] == CHR_PV)
		{
			if (j != *i + 1)
				type = CHR_ERROR;
		}
	}
	else
		while (g_get_chr[(int)str[*i]] == g_get_chr[(int)str[j]])
		{
			if (str[j]== '\\' && str[j + 1])
				j++;
			if (str[j])
				j++;
			else
				type = CHR_ERROR;
		}
	if (g_get_chr[(int)str[*i]] != CHR_SP)
		value = ft_strndup(str + *i, j - *i);
	else
		value = NULL;
	if (type != CHR_ERROR)
		type = g_get_chr[(int)str[*i]];
	*i = j;
	if (g_get_chr[(int)str[j]] != CHR_END)
		tab = lexer(str, i, lvl + 1);
	else
	{
		tab = (t_tok**)malloc(sizeof(t_tok*) * (lvl + 2));
		tab[lvl + 1] = create_tok(CHR_END, "newline");
	}	
	tab[lvl] = create_tok(type, value);	
	return (tab);
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

int		token_push_back(t_tok **begin, t_tok *token)
{
	t_tok *tmp;

	if (!begin || !token)
		return (0);
	if (!(*begin))
	{
		*begin = token;
		return (1);
	}
	tmp = (*begin);
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	return (1);
}

t_tok	*get_env_var_tok(const char *value)
{
	t_tok	*tok_var;
	t_tok	*token;
	char	*str;
	int		i;
	int		j;

	tok_var = NULL;
	if (!value)
	{
		tok_var = create_tok(CHR_ST, NULL);
		return (tok_var);
	}
	if (!*value)
	{
		tok_var = create_tok(CHR_ST, ft_strdup(""));
		return (tok_var);
	}
	i = 0;
	while (value[i])
	{
		if (ft_isspace(value[i]))
		{
			token = create_tok(CHR_SP, NULL);
			if (!token_push_back(&tok_var, token))
				return (NULL);
			while (ft_isspace(value[i]))
				i++;
		}
		else
		{
			j = i;
			while (value[j] && !ft_isspace(value[j]))
				j++;
			str = ft_strndup(value + i, j - i);
			if (!str)
				return (NULL);
			token = create_tok(CHR_ST, str);
			if (!token_push_back(&tok_var, token))
				return (NULL);
			i = j;
		}
	}
	return (tok_var);
}

t_tok	*get_word_tok(t_tok	*tok_lex, t_cmd	*cmd)
{
	char	*str;
	t_tok	*tok_word;
	t_tok	*token;
	t_dlist	*elem;
	char	*var_name;
	int		i;
	int		j;
	int		k;
	t_tok	*tmp;

	str = ft_strndup(tok_lex->value, ft_strlen((char*)tok_lex->value));
	// printf("get_word str->%s\n", str);
	if (!str)
		return (NULL);
	i = 0;
	k = 0;
	tok_word = NULL;
	while (str[i])
	{
		if (str[i] == '\\')
        {
            str = insert_string(str, strdup(""), i, i + 1);
			if (!str)
				return (NULL);
		}
		else if (str[i] == '$')
		{
			j = i + 1;
			while (ft_isalnum(str[j]) || str[j] == '_')
				j++;
			if (j == i + 1)
			{
				i++;
				continue;
			}
			if (i - k > 0)
			{
				token = create_tok(CHR_ST, ft_strndup(str + k, i - k));
				if (!token)
					return (NULL);
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			var_name = ft_strndup(str + i + 1, j - (i + 1));
			if (!var_name)
				return (NULL);
			elem = dlist_chr(cmd->envlist, var_name);
			if (!elem)
			{
				token = create_tok(CHR_ST, NULL);
				if (!token)
					return (NULL);
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			else
			{
				// printf("elem ->%p\n", elem);
				// printf("data ->%p\n", elem->data);
				// printf("value ->%p\n", elem->data->value);
				token = get_env_var_tok((char*)elem->data->value);
				// printf("wesh\n");
				if (!token)
					return (NULL);
				tmp = token;
				// printf("*********************** get env var tok ****************************\n");
				// while (tmp)
				// {
				// 	printf ("value->%s; type->%d\n", tmp->value, tmp->type);
				// 	tmp = tmp->next;
				// }
				// printf("@@@@@@@@@@@@@@@@@@@@@@@ get env var tok @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
				if (!token_push_back(&tok_word, token))
					return (NULL);
			}
			i = j - 1;
			k = j;
		}
		i++;
	}
	// printf("end tok_word str->%s\n", str);
	if (i - k > 0)
	{
		token = create_tok(CHR_ST, ft_strndup(str + k, i - k));
		// if (token)
		// 	printf("tok_word created ->%s\n", token->value);
		if (!token)
			return (NULL);
		if (!token_push_back(&tok_word, token))
			return (NULL);
	}
	return (tok_word);
}

t_tok	**get_tok_arg(t_tok **tok_lex, t_cmd *cmd)
{
	char 	*str;
	t_tok	*token;

	if (!tok_lex || !(*tok_lex))
		return (NULL);
	while ((*tok_lex)->type > CHR_ERROR && (*tok_lex)->type < CHR_PV)
		{
			// printf("tok_lex:%s\n", (*tok_lex)->value);
			if ((*tok_lex)->type == CHR_SP)
			{
				token = create_tok(CHR_SP, NULL);
				if (token)
				{
					if (!token_push_back(&cmd->tok_arg, token))
						return (NULL);
				}
				else
					return (NULL);
			}
			else if ((*tok_lex)->type == CHR_ST)
			{
				// if (ft_strlen((*tok_lex)->value) <= 2)
				// {
				// 	tok_lex++;
				// 	continue;
				// }
				str = ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2);
				if (str)
				{
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else	
						return (NULL);
				}
				else
					return (NULL);
				
			}
			else if ((*tok_lex)->type == CHR_STR)
			{
				// if (ft_strlen((*tok_lex)->value) <= 2)
				// {
				// 	tok_lex++;
				// 	continue;
				// }
				str = get_str(ft_strndup((*tok_lex)->value + 1, ft_strlen((*tok_lex)->value) - 2), cmd->envlist);
				if (str)
				{
					// if (!*str)
					// {
					// 	free(str);
					// 	tok_lex++;
					// 	continue;
					// }
					token = create_tok(CHR_ST, str);
					if (token)
						token_push_back(&cmd->tok_arg, token);
					else
						return (NULL);
				}
				else
				{
					return (NULL);
				}
				
			}
			else if ((*tok_lex)->type == CHR_WORD)
			{
				token = get_word_tok(*tok_lex, cmd);
				// if (token)
				// {
				// 	printf("tok_arg token->%s\n", token->value);
				// }
				// tmp = token;
				// printf("*************************************** get word tok ****************************\n");
				// while (tmp)
				// {
				// 	printf ("value->%s; type->%d\n", tmp->value, tmp->type);
				// 	tmp = tmp->next;
				// }
				// printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ get word tok @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
			}
			else
			{
				token = create_tok((*tok_lex)->type, ft_strdup((*tok_lex)->value));
				if (token)
					token_push_back(&cmd->tok_arg, token);
				else
					return (NULL);
				
			}
			
			tok_lex++;
		}
		token_push_back(&cmd->tok_arg, create_tok(CHR_END, "newline"));
		return (tok_lex);
}

void	get_ac_av(t_tok *tok_lst,  t_cmd *cmd, int lvl)
{
	int		i;
	char	*s1 = NULL;
	char	*s2 = NULL;

	i = 0;
	if (!tok_lst)
		return;
	if (tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
	{
		while (tok_lst->type == CHR_SP)
			tok_lst = tok_lst->next;
		while (tok_lst && tok_lst->type == CHR_ST)
		{
			if (tok_lst->value)
			{
				s2 = ft_strdup((char*)tok_lst->value);
				s1 = ft_strjoindoublefree(s1, s2);
			}
			tok_lst = tok_lst->next;
		}
		if (s1 && tok_lst->type != CHR_END && tok_lst->type > CHR_ERROR &&  tok_lst->type < CHR_OP)
			get_ac_av(tok_lst->next, cmd, lvl + 1);
		else if (s1)
			get_ac_av(tok_lst, cmd, lvl + 1);
		else
			get_ac_av(tok_lst, cmd, lvl);

	}
	else
	{
		cmd->av = (char**)malloc(sizeof(char*) * (lvl + 1));
		cmd->av[lvl] = NULL;
		cmd->ac = lvl;
		return;
	}
	if (s1)
		cmd->av[lvl] = s1;
	return;
}