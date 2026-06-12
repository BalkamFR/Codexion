/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:45:01 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/12 23:12:56 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

static void	take_dongle(t_coder **coder, int i, t_data *setting)
{
	if ((*coder)[i].id % 2 == 0)
	{
		(*coder)[i].left_dongle = &setting->dongles[i];
		if (i < setting->number_of_coders - 1)
			(*coder)[i].right_dongle = &setting->dongles[i + 1];
		else
			(*coder)[i].right_dongle = &setting->dongles[0];
	}
	else
	{
		(*coder)[i].right_dongle = &setting->dongles[i];
		if (i < setting->number_of_coders - 1)
			(*coder)[i].left_dongle = &setting->dongles[i + 1];
		else
			(*coder)[i].left_dongle = &setting->dongles[0];
	}
}

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
		take_dongle(coder, i, setting);
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
