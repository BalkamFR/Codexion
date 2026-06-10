/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:45:01 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/10 14:53:26 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

int	init_all_coder(t_data *setting, t_coder **coder)
{
	int	i;

	i = 0;
	while (i < setting->number_of_coders)
	{
		(*coder)[i].data = setting;
		(*coder)[i].id = i + 1;
		(*coder)[i].last_compile_start = get_time();
		(*coder)[i].compile_count = 0;
		(*coder)[i].left_dongle = &setting->dongles[i];
		if (i < setting->number_of_coders - 1)
			(*coder)[i].right_dongle = &setting->dongles[i + 1];
		else
			(*coder)[i].right_dongle = &setting->dongles[0];
		i++;
	}
	return (0);
}

int	create_all_coder(t_coder *coder, t_data *setting, void *(*routine)(void *))
{
	int	i;

	i = 0;
	while (i < setting->number_of_coders)
	{
		pthread_create(&coder[i].thread, NULL, routine, &coder[i]);
		i++;
	}
	return (0);
}
void	*monitor(void *arg)
{
	int			i;
	int			all_done;
	long int	time;
	t_coder		*coders;

	coders = (t_coder *)arg;
	while (!coders[0].data->stop_sim)
	{
		i = 0;
		all_done = 1;
		while (i < coders->data->number_of_coders)
		{
			if (coders[i].compile_count < coders->data->number_of_compiles_required)
				all_done = 0;
			if (get_time()
				- coders[i].last_compile_start > coders[i].data->time_to_burnout)
			{
				pthread_mutex_lock(&coders->data->write_mutex);
				coders->data->stop_sim = 1;
				time = get_time() - coders[i].data->start_time;
				printf("%ld %d %s\n", time, coders[i].id, "burned out");
				pthread_mutex_unlock(&coders->data->write_mutex);
				return (NULL);
			}
			i++;
		}
		if (all_done == 1)
		{
			pthread_mutex_lock(&coders->data->write_mutex);
			coders[0].data->stop_sim = 1;
			pthread_mutex_unlock(&coders->data->write_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
