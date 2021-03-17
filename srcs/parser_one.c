/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admadene <admadene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 14:32:13 by admadene          #+#    #+#             */
/*   Updated: 2021/03/17 14:40:40 by admadene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_st(char *str)
{
	int	i;

	if (*str != '\'')
		return (NULL);
	i = 0;
	while (str[i])
		i++;
	return (ft_strndup(str + 1, i - 2));
}

char	*get_var(char *str, int *i, int *j, t_dlist *envlist)
{
	char	*ptr;
	t_dlist	*elem;

	(*j) = (*i) + 1;
	while (ft_isalpha(str[(*j)]) || ft_isdigit(str[(*j)]) ||\
			str[(*j)] == '_' || str[*j] == '?')
		(*j)++;
	ptr = ft_strndup(str + (*i) + 1, (*j) - ((*i) + 1));
	if ((elem = dlist_chr(envlist, ptr)) &&\
			elem->data->value && elem->data->value[0])
	{
		str = insert_string(str, ft_strdup(elem->data->value), (*i), (*j));
		(*i) += ft_strlen(elem->data->value) - 1;
	}
	else
	{
		str = insert_string(str, strdup(""), (*i), (*j));
		(*i)--;
	}
	free(ptr);
	return (str);
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
			str = get_var(str, &i, &j, envlist);
		i++;
	}
	return (str);
}

char	*get_word(char *str, t_dlist *envlist)
{
	int		i;
	int		j;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			str = insert_string(str, strdup(""), i, i + 1);
		else if (str[i] == '$')
			get_var(str, &i, &j, envlist);
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
