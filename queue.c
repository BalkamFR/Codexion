/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:36:30 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/04 14:37:21 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	pop_queue(t_data *data)
{
	t_node	*tmp;

	if (!data->queue)
		return (1);
	tmp = data->queue;
	data->queue = tmp->next;
	free(tmp);
	return (0);
}

int	push_to_queue(t_data *data, t_coder *coder)
{
	t_node *new_node;
	t_node *tmp;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (1);
	new_node->coder = coder;
	new_node->next = NULL;
	if (!data->queue)
		data->queue = new_node;
	else
	{
		tmp = data->queue;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (0);
}