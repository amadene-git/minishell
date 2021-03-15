/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmpci.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 12:46:55 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/15 12:47:23 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

int		ft_strcmpci(const char *s1, const char *s2)
{
	char	l1;
	char	l2;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return ((int)(-*s2));
	if (!s2)
		return ((int)*s1);
	l1 = ft_tolower(*s1);
	l2 = ft_tolower(*s2);
	while (*s1 && *s2 && !(l1 - l2))
	{
		s1++;
		s2++;
		l1 = ft_tolower(*s1);
		l2 = ft_tolower(*s2);
	}
	return ((int)(l1 - l2));
}
