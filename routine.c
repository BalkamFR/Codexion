/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:42:12 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/26 17:16:52 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

static int	is_blocked(t_coder *coder, int left, int right)
{
	t_data	*data;

	data = coder->data;
	if (data->queue->coder != coder)
		return (1);
	if (data->dongle_status[left] == 1 || data->dongle_status[right] == 1)
		return (1);
	if (get_time() - data->dongle_last_released[left] < data->dongle_cooldown)
		return (1);
	if (get_time() - data->dongle_last_released[right] < data->dongle_cooldown)
		return (1);
	return (0);
}

void	routine_pthread_mutex_dongles(t_coder *coder, char *status, int right,
		int left)
{
	if (ft_strcmp(status, "lock"))
	{
		if (coder->data->number_of_coders > 1)
		{
			pthread_mutex_lock(coder->right_dongle);
			coder->data->dongle_status[right] = 1;
			print_status(coder, "has taken a dongle");
		}
		pthread_mutex_lock(coder->left_dongle);
		coder->data->dongle_status[left] = 1;
		print_status(coder, "has taken a dongle");
	}
	else if (ft_strcmp(status, "unlock"))
	{
		if (coder->data->number_of_coders > 1)
			pthread_mutex_unlock(coder->right_dongle);
		pthread_mutex_unlock(coder->left_dongle);
		coder->data->dongle_status[right] = 0;
		coder->data->dongle_status[left] = 0;
		coder->data->dongle_last_released[left] = get_time();
		coder->data->dongle_last_released[right] = get_time();
	}
}

void	wait_in_queue(t_coder *coder, int right, int left)
{
	while (is_blocked(coder, left, right) && !coder->data->stop_sim)
		pthread_cond_wait(&coder->data->queue_cond, &coder->data->queue_mutex);
}

void	debug_and_refactor(t_coder *coder)
{
	print_status(coder, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
	print_status(coder, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	int		left;
	int		right;
	int		stopped;

	coder = (t_coder *)arg;
	left = coder->id - 1;
	right = coder->id % coder->data->number_of_coders;
	pthread_mutex_lock(&coder->data->stop_mutex);
	stopped = coder->data->stop_sim;
	pthread_mutex_unlock(&coder->data->stop_mutex);
	routine_while(coder, stopped, left, right);
	return (NULL);
}
