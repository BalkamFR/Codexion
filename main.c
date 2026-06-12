/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pacome <pacome@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:55:42 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/12 14:20:36 by pacome           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"


int	is_blocked(t_coder *coder, int left, int right)
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
        while (is_blocked(coder, left, right) && !coder->data->stop_sim)
			pthread_cond_wait(&coder->data->queue_cond, &coder->data->queue_mutex);
		if (coder->data->stop_sim)
		{
			pthread_mutex_unlock(&coder->data->queue_mutex);
			return (NULL);
		}
		pthread_mutex_lock(coder->left_dongle);
		pthread_mutex_lock(coder->right_dongle);
		coder->data->dongle_status[right] = 1;
		coder->data->dongle_status[left] = 1;
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
		pop_queue(coder->data);
		pthread_mutex_unlock(&coder->data->queue_mutex);
		print_status(coder, "is compiling");
		coder->last_compile_start = get_time();
		usleep(coder->data->time_to_compile * 1000);
		coder->compile_count ++;
		pthread_mutex_lock(&coder->data->queue_mutex);
		pthread_mutex_unlock(coder->right_dongle);
		pthread_mutex_unlock(coder->left_dongle);
		coder->data->dongle_status[right] = 0;
		coder->data->dongle_status[left] = 0;
        coder->data->dongle_last_released[left] = get_time();
        coder->data->dongle_last_released[right] = get_time();
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
    int     i;

    i = 0;

	setting = parser(argv + 1);
	if (!setting)
		return (1);
	setting->start_time = get_time();
	coder = malloc(sizeof(t_coder) * setting->number_of_coders);
	if (!coder)
		return (1);
	memset(setting->dongle_status, 0, sizeof(int) * setting->number_of_coders);
	init_all_coder(setting, &coder);
	create_all_coder(coder, setting, routine);
	pthread_create(&moni, NULL, monitor, coder);
	join_all_coder(setting, coder);
	pthread_join(moni, NULL);
    while (i < setting->number_of_coders)
    {
        pthread_mutex_destroy(&setting->dongles[i]);
        i++;
    }
    pthread_mutex_destroy(&setting->queue_mutex);
    pthread_mutex_destroy(&setting->stop_mutex);
    pthread_mutex_destroy(&setting->write_mutex);
    pthread_cond_destroy(&setting->queue_cond);
    while (setting->queue)
        pop_queue(setting);
    free(setting->dongle_status);
    free(setting->dongle_last_released);
    free(setting->dongles);
    free(coder);
    free(setting);

	(void)argc;
	(void)argv;
	return (0);
}
