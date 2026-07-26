/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_coder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 23:13:48 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/25 14:08:42 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	join_all_coder(t_data *setting, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < setting->number_of_coders)
	{
		pthread_join(coder[i].thread, NULL);
		i++;
	}
}

void	print_status(t_coder *coder, char *status)
{
	long int	time;

	pthread_mutex_lock(&coder->data->write_mutex);
	if (coder->data->stop_sim == 1)
	{
		pthread_mutex_unlock(&coder->data->write_mutex);
		return ;
	}
	time = get_time() - coder->data->start_time;
	printf("%ld %d %s\n", time, coder->id, status);
	pthread_mutex_unlock(&coder->data->write_mutex);
}

void	routine_compiling(t_coder *coder)
{
	pop_queue(coder->data);
	pthread_mutex_unlock(&coder->data->queue_mutex);
	coder->last_compile_start = get_time();
	print_status(coder, "is compiling");
	usleep(coder->data->time_to_compile * 1000);
	coder->compile_count++;
	pthread_mutex_lock(&coder->data->queue_mutex);
}
