/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/14 22:37:47 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/14 22:41:29 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		built_in_pwd(int ac, char **av)
{
	char	buf[3200];

	(void)av;
	(void)ac;
	if (!getcwd(&buf[0], 31999))
	{
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	ft_putendl_fd(&buf[0], 1);
	return (0);
}
