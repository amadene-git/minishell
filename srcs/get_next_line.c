/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 19:14:50 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/01/30 20:29:27 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t i;

	i = 0;
	while (src[i] != '\0' && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char			*p;
	size_t			i;
	unsigned char	mychar;

	mychar = (unsigned char)c;
	p = (char *)b;
	i = 0;
	while (i < len)
	{
		p[i] = mychar;
		i++;
	}
	return (b);
}

int	ft_chrloc(const char *str, int ch)
{
	size_t		length;

	length = 0;
	if (str)
		while (str[length] != ch && str[length] != '\0')
			length++;
	if (length == 0 && str[0] != ch)
		return (-1);
	return (length);
}

char	*ft_strnew(size_t size)
{
	char	*str;

	if (!(str = (char *)malloc(size + 1)))
		return (NULL);
	return (ft_memset(str, 0, size + 1));
}

char			*ft_strnjoin(const char *s1, const char *s2, int len)
{
	char	*s;
	char	*ret;

	if (len == -1)
		len = 0;
	if (!(s = ft_strnew(ft_chrloc(s1, '\0') + len)))
		return (NULL);
	ret = s;
	while (s1 && *s1)
		*s++ = *s1++;
	while (*s2 && s2 && len--)
		*s++ = *s2++;
	*s = '\0';
	return (ret);
}

static	int		check_errors(int const fd, char **line)
{
	if (BUFFER_SIZE <= 0 || fd < 0 || line == NULL ||
			fd > OPEN_MAX || !(*line = ft_strnew(1)))
		return (-1);
	return (0);
}

static	int		get_rest_buffer(char buff[BUFFER_SIZE + 1], int len, char *line)
{
	if (!buff[0] && line[0])
		buff[0] = '\0';
	else
	{
		if (ft_chrloc(buff, '\n') != -1)
		{
			ft_strncpy(buff, &buff[len + 1], BUFFER_SIZE + 1);
			return (1);
		}
		else
			ft_memset(buff, 0, ft_chrloc(buff, '\0'));
	}
	return (0);
}

int				get_next_line(int const fd, char **line)
{
	int			errorsize;
	int			len;
	char		*str;
	static char	buff[OPEN_MAX][BUFFER_SIZE + 1];

	if (check_errors(fd, line) == -1)
		return (-1);
	errorsize = 1;
	while (errorsize > 0)
	{
		if (!buff[fd][0])
			errorsize = read(fd, &buff[fd], BUFFER_SIZE);
		if (errorsize < 0)
			return (errorsize);
		len = ft_chrloc(buff[fd], '\n');
		str = *line;
		if (!(*line = ft_strnjoin(str, buff[fd], len)))
			return (-1);
		free(str);
		str = NULL;
		if ((*line && buff[fd][len]) || (!buff[fd][0] && *line[0]))
			return (get_rest_buffer(buff[fd], len, *line));
		ft_memset(buff[fd], 0, ft_chrloc(buff[fd], '\0'));
	}
	return (0);
}
