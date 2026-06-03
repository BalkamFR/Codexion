/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:02:34 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/03 18:58:17 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes/codexion.h"

int	ft_strlen(char *tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	write_error(char *tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		write(2, &tab[i], 1);
		i++;
	}
	write(2, "\n", 1);
}
int	ft_is_number(char number)
{
	if (number >= '0' && number <= '9')
		return (1);
	return (0);
}

void join_all_coder(t_data *setting, t_coder *coder)
{
	int i;

	i = 0;
	while (i < setting->number_of_coders)
	{
		pthread_join(coder[i].thread, NULL);
		i++;
	}
}
long int get_time(void)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void    print_status(t_coder *coder, char *status)
{
	long int time;
	pthread_mutex_lock(&coder->data->write_mutex);
	if (coder->data->stop_sim == 1)
	{
		pthread_mutex_unlock(&coder->data->write_mutex);
		return;
	}
	time = get_time() - coder->data->start_time;
	printf("%ld %d %s\n", time, coder->id, status);
	pthread_mutex_unlock(&coder->data->write_mutex);
}