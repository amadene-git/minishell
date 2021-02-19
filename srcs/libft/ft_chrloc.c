/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chrloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 00:03:44 by mbouzaie          #+#    #+#             */
/*   Updated: 2020/10/11 15:09:58 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

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
