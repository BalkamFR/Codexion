/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:30:01 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/26 17:16:59 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	*routine_while(t_coder *coder, int stopped, int left, int right)
{
	while (!stopped)
	{
		pthread_mutex_lock(&coder->data->queue_mutex);
		push_to_queue(coder->data, coder);
		wait_in_queue(coder, right, left);
		pthread_mutex_lock(&coder->data->stop_mutex);
		stopped = coder->data->stop_sim;
		pthread_mutex_unlock(&coder->data->stop_mutex);
		if (stopped)
		{
			pthread_mutex_unlock(&coder->data->queue_mutex);
			return (NULL);
		}
		routine_pthread_mutex_dongles(coder, "lock", right, left);
		routine_compiling(coder);
		routine_pthread_mutex_dongles(coder, "unlock", right, left);
		pthread_cond_broadcast(&coder->data->queue_cond);
		pthread_mutex_unlock(&coder->data->queue_mutex);
		debug_and_refactor(coder);
		pthread_mutex_lock(&coder->data->stop_mutex);
		stopped = coder->data->stop_sim;
		pthread_mutex_unlock(&coder->data->stop_mutex);
	}
	return (NULL);
}
