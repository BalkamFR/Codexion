/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:55:42 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/26 17:16:29 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

static void	cleanup_all(t_data *setting, t_coder *coder)
{
	int	i;

	i = 0;
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
}

static int	run_simulation(t_data *setting)
{
	t_coder		*coder;
	pthread_t	moni;

	setting->start_time = get_time();
	coder = malloc(sizeof(t_coder) * setting->number_of_coders);
	if (!coder)
	{
		free(setting->dongle_status);
		free(setting->dongle_last_released);
		free(setting->dongles);
		free(setting);
		return (1);
	}
	memset(setting->dongle_status, 0, sizeof(int) * setting->number_of_coders);
	init_all_coder(setting, &coder);
	create_all_coder(coder, setting, routine);
	pthread_create(&moni, NULL, monitor, coder);
	join_all_coder(setting, coder);
	pthread_join(moni, NULL);
	cleanup_all(setting, coder);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*setting;

	if (argc != 9 || atoi(argv[1]) <= 0)
	{
		write_error("");
		return (1);
	}
	setting = parser(argv + 1);
	if (!setting)
		return (1);
	if (run_simulation(setting))
		return (1);
	return (0);
}
