/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 18:05:22 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/03/13 16:21:19 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_minishell(int  status)
{
	exit(status);
}

int		exit_cmd(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (cmd->ac == 1)
		exit_minishell(0);
	if (!is_number(cmd->av[1]) || (ft_atoi(cmd->av[1]) == -1 && !is_minus_one(cmd->av[1]))\
		|| (ft_atoi(cmd->av[1]) == 0 && !is_zero(cmd->av[1])))
	{
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", cmd->av[1]);
		exit_minishell(255);
	}
	else if (cmd->ac > 2)
	{
		ft_dprintf(2,"minishell: exit: too many arguments\n");
		status = 1;
	}
	else if (cmd->ac == 2)
		exit_minishell(ft_atoi(cmd->av[1]));
	return (status);
	
}
