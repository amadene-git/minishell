/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 14:26:32 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/19 14:26:51 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strndup(const char *str, int n)
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
