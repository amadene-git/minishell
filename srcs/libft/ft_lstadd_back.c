/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/05 22:26:21 by mbouzaie          #+#    #+#             */
/*   Updated: 2021/02/19 13:38:02 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	ft_lstadd_back(t_list **alst, t_list *lnew)
{
	t_list	*temp;

	if (alst && lnew)
	{
		if (*alst)
		{
			temp = *alst;
			while (temp->next)
			{
				temp = temp->next;
			}
			temp->next = lnew;
		}
		else
		{
			*alst = lnew;
		}
	}
}
