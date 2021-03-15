/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_minus_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 13:17:33 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/15 13:17:47 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

int		ft_isminusone(const char *str)
{
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		str++;
	else
		return (0);
	while (*str == '0')
		str++;
	if (*str == '1')
		return (1);
	return (0);
}
