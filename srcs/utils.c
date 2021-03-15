/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:27:05 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/15 13:37:21 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoindoublefree(char *s1, char *s2)
{
	char	*str;
	int		len;
	char	*cpy;
	char	*cpy2;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (!s2 && s1)
		return (ft_strdup(s1));
	if (!s1 && !s2)
		return (NULL);
	cpy = s1;
	cpy2 = s2;
	len = ft_strlen((char*)s1) + ft_strlen((char*)s2);
	str = (char*)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	while (*s1)
		*str++ = *s1++;
	while (*s2)
		*str++ = *s2++;
	*str = '\0';
	free(cpy2);
	free(cpy);
	return (str - len);
}

char	*insert_string(char *str, char *to_insert, int start, int end)
{
	char	*ret;
	int		i;

	if (!(ret = (char*)ft_calloc(sizeof(char), (ft_strlen(str) \
	+ ft_strlen(to_insert) + (end - start) + 1))))
		return (NULL);
	i = -1;
	while (++i < start && str[i])
		ret[i] = str[i];
	while ((size_t)i < start + ft_strlen(to_insert) && to_insert[i - start])
	{
		ret[i] = to_insert[i - start];
		i++;
	}
	while (str[i - ft_strlen(to_insert) + (end - start)])
	{
		ret[i] = str[i - ft_strlen(to_insert) + (end - start)];
		i++;
	}
	ret[i] = 0;
	free(str);
	free(to_insert);
	return (ret);
}

int		is_number(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && !ft_isblank(str[i]))
			return (0);
		if (ft_isblank(str[i]))
			break ;
		i++;
	}
	while (str[i])
	{
		if (!ft_isblank(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int		is_valid_name(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		if (ft_isalpha(*str) || *str == '_')
			i = 1;
		str++;
	}
	return (i);
}
