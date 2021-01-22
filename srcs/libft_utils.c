#include "minishell.h"

char	*insert_string(char *str, char *to_insert, int start, int end)
{
	char	*ret;
	int		i;

	if (!(ret = (char*)ft_calloc(sizeof(char), (ft_strlen(str) \
	+ ft_strlen(to_insert) + (end - start) + 1))))
		return (NULL);
	i = -1;
	while (++i < start)
		ret[i] = str[i];
	while ((size_t)i < start + ft_strlen(to_insert))
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

int		ft_strcmp(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return ((int)(*s2 *= -1));
	if (!s2)
		return ((int)*s1);
	while (*s1 && *s2 && !(*s1 - *s2))
	{
		s1++;
		s2++;
	}
	return ((int)(*s1 - *s2));
}

char	*ft_strndup(char *str, int n)
{
	int		len;
	int		i;
	char	*dup;

	len = ft_strlen(str);
	len = (n > len) ? len : n;
	if (!(dup = (char*)malloc(len + 1)))
		return (NULL);
	i = -1;
	while (++i < len)
		dup[i] = str[i];
	dup[i] = 0;
	return (dup);
}
