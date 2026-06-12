/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:36:30 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/12 23:20:26 by papilaz          ###   ########.fr       */
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

static void	queue_edf(t_data *data, t_coder *coder, t_node *new_node)
{
	long int	new_deadline;
	t_node		*tmp;

	new_deadline = coder->last_compile_start + data->time_to_burnout;
	if (new_deadline < data->queue->coder->last_compile_start
		+ data->time_to_burnout)
	{
		new_node->next = data->queue;
		data->queue = new_node;
	}
	else
	{
		tmp = data->queue;
		while (tmp->next && (tmp->next->coder->last_compile_start
				+ data->time_to_burnout) <= new_deadline)
			tmp = tmp->next;
		new_node->next = tmp->next;
		tmp->next = new_node;
	}
}

static void	queue_fifo(t_data *data, t_node *new_node)
{
	t_node	*tmp;

	tmp = data->queue;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int	push_to_queue(t_data *data, t_coder *coder)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (1);
	new_node->coder = coder;
	new_node->next = NULL;
	if (!data->queue)
		data->queue = new_node;
	else if (ft_strcmp(data->scheduler, "fifo"))
		queue_fifo(data, new_node);
	else if (ft_strcmp(data->scheduler, "edf"))
		queue_edf(data, coder, new_node);
	return (0);
}
