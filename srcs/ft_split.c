/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 21:59:46 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/02 15:21:25 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len < start)
	{
		if (!(str = malloc(sizeof(char) * 1)))
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (i < len && s[i + start])
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static int		ft_count_word(const char *s, char c)
{
	int		words;
	int		is_word;

	is_word = 0;
	words = 0;
	while (*s != '\0')
	{
		if (is_word == 1 && *s == c)
			is_word = 0;
		if (is_word == 0 && *s != c)
		{
			is_word = 1;
			words++;
		}
		s++;
	}
	return (words);
}

static int		ft_wordlen(const char *s, char c)
{
	int		len;

	len = 0;
	while (*s != c && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

void			*free_tab(char **strs)
{
	if (strs == NULL)
		return (NULL);
	while (*strs != NULL)
		free(*strs++);
	free(strs);
	return (NULL);
}

char			**ft_split(char const *s, char c)
{
	char	**w;
	int		words;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	words = ft_count_word(s, c);
	if (!(w = (char**)malloc(sizeof(char*) * (words + 1))))
		return (NULL);
	while (words > 0)
	{
		while (*s == c && *s != '\0')
			s++;
		w[i] = ft_substr((const char *)s, 0, ft_wordlen((const char *)s, c));
		if (w[i] == NULL)
			return (free_tab(w));
		s = s + ft_wordlen((const char *)s, c);
		i++;
		words--;
	}
	w[i] = NULL;
	return (w);
}
