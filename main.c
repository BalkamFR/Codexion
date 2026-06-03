/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:55:42 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/03 20:51:41 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"
void	*routine(void *arg)	
{
	t_coder	*coder;
	int left;
	int right;

	coder = (t_coder *)arg;
	left = coder-> id - 1;
	right = coder->id % coder->data->number_of_coders;
	while (!coder->data->stop_sim)
	{
		pthread_mutex_lock(&coder->data->queue_mutex);
		push_to_queue(coder->data, coder);
		while (coder->data->queue->coder != coder || 
			coder->data->dongle_status[left] == 1 || 
			coder->data->dongle_status[right] == 1)
			pthread_cond_wait(&coder->data->queue_cond, &coder->data->queue_mutex);
		coder->data->dongle_status[right] = 1;
		coder->data->dongle_status[left] = 1;
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
		pop_queue(coder->data);
		pthread_mutex_unlock(&coder->data->queue_mutex);
		print_status(coder, "is compiling");
		coder->last_compile_start = get_time();
		usleep(coder->data->time_to_compile * 1000);
		pthread_mutex_lock(&coder->data->queue_mutex);
		coder->data->dongle_status[right] = 0;
		coder->data->dongle_status[left] = 0;
		pthread_cond_broadcast(&coder->data->queue_cond);
		pthread_mutex_unlock(&coder->data->queue_mutex);
		print_status(coder, "is debugging");
		usleep(coder->data->time_to_debug * 1000);
		print_status(coder, "is refactoring");
		usleep(coder->data->time_to_refactor * 1000);
	}
	return (NULL);
}


int	main(int argc, char **argv)
{
	t_data *setting;
	t_coder *coder;
	pthread_t moni;

	setting = parser(argv + 1);
	if (!setting)
		return (1);
	setting->start_time = get_time();
	coder = malloc(sizeof(t_coder) * setting->number_of_coders);
	if (!coder)
		return (1);
	memset(setting->dongle_status, 0, sizeof(int) * setting->number_of_coders);
	init_all_coder(setting, &coder);
	pthread_create(&moni, NULL, monitor, coder);
	create_all_coder(coder, setting, routine);
	join_all_coder(setting, coder);
	pthread_join(moni, NULL);
	(void)argc;
	(void)argv;
	return (0);
}