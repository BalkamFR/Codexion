/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pacome <pacome@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:36:30 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/12 14:11:14 by pacome           ###   ########.fr       */
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
    long int new_deadline;
    long int current_deadline;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
    return (1);
    new_node->coder = coder;
    new_node->next = NULL;
    new_deadline = coder->last_compile_start + data->time_to_burnout;
    if (!data->queue)
    data->queue = new_node;
    else if (verif_scheduler(data->scheduler, "fifo"))
    {
        tmp = data->queue;
		while (tmp->next)
        tmp = tmp->next;
		tmp->next = new_node;
    }
    else if (verif_scheduler(data->scheduler, "edf"))
    {
        current_deadline = data->queue->coder->last_compile_start + data->time_to_burnout;
        if (new_deadline < current_deadline)
        {
            new_node->next = data->queue;
            data->queue = new_node;
        }
        else
        {
            tmp = data->queue;
            while (tmp->next && current_deadline <= new_deadline)
            {
                current_deadline = tmp->next->coder->last_compile_start + data->time_to_burnout;
                if (current_deadline > new_deadline)
                    break;
                tmp = tmp->next;
            }
            new_node->next = tmp->next;
            tmp->next = new_node;
        }
        
    }
	return (0);
}
