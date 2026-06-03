/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:45:01 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/03 21:13:21 by papilaz          ###   ########.fr       */
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
		i++;
	}
	return (0);
}

int create_all_coder(t_coder *coder, t_data *setting, void *(*routine)(void *))
{
    int i ;

    i = 0;
    while (i < setting->number_of_coders)
    {
        pthread_create(&coder[i].thread, NULL, routine, &coder[i]);
        i++;
    }
    return 0;
}

int 	pop_queue(t_data *data)
{
	t_node *tmp;
	if (!data->queue)
		return 1;
	
	tmp = data->queue;
	data->queue = tmp->next;
	free(tmp);
	return 0;
}

int	push_to_queue(t_data *data, t_coder *coder)
{
	t_node *new_node;
	t_node *tmp;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return 1;
	new_node->coder = coder;
	new_node->next = NULL;
	if (!data->queue)
		data->queue = new_node;
	else
	{
		tmp = data->queue;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return 0;
}
void    *monitor(void *arg)
{
	int  i;
	long int time;

	t_coder *coders = (t_coder *)arg;
	while (!coders[0].data->stop_sim)
	{
		i = 0;
		while (i < coders->data->number_of_coders)
		{
			if (get_time() - coders[i].last_compile_start > coders[i].data->time_to_burnout)
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
		usleep(1000);
	}
	return (NULL);
}
