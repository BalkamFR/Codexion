/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:50:38 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/26 17:16:47 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

static int	monitor_check_burnout(t_coder *coders, int i)
{
	long int	time;

	if (get_time()
		- coders[i].last_compile_start > coders[i].data->time_to_burnout)
	{
		pthread_mutex_lock(&coders->data->write_mutex);
		coders->data->stop_sim = 1;
		time = get_time() - coders[i].data->start_time;
		printf("%ld %d %s\n", time, coders[i].id, "burned out");
		pthread_mutex_unlock(&coders->data->write_mutex);
		pthread_cond_broadcast(&coders->data->queue_cond);
		return (1);
	}
	return (0);
}

static int	monitor_check_all_done(t_coder *coders, int all_done)
{
	if (all_done == 1)
	{
		pthread_mutex_lock(&coders->data->write_mutex);
		coders[0].data->stop_sim = 1;
		pthread_mutex_unlock(&coders->data->write_mutex);
		pthread_cond_broadcast(&coders->data->queue_cond);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	int		i;
	int		all_done;
	t_coder	*coders;
	int		number_compiles_required;

	coders = (t_coder *)arg;
	number_compiles_required = coders->data->number_of_compiles_required;
	while (!coders[0].data->stop_sim)
	{
		i = 0;
		all_done = 1;
		while (i < coders->data->number_of_coders)
		{
			if (coders[i].compile_count < number_compiles_required)
				all_done = 0;
			if (monitor_check_burnout(coders, i))
				return (NULL);
			i++;
		}
		if (monitor_check_all_done(coders, all_done))
			return (NULL);
		pthread_cond_broadcast(&coders->data->queue_cond);
		usleep(1000);
	}
	return (NULL);
}
