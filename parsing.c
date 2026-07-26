/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:48:48 by papilaz           #+#    #+#             */
/*   Updated: 2026/07/25 14:12:10 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

static int	verif_argv(char **argv)
{
	int	i;
	int	a;

	i = 0;
	a = 0;
	while (i <= 6)
	{
		while (argv[i][a])
		{
			if (!ft_is_number(argv[i][a]) || atoi(argv[i]) < 0)
			{
				write_error("[Error] negative value or str is forbiden\n");
				return (0);
			}
			a++;
		}
		a = 0;
		i++;
	}
	if (!ft_strcmp(argv[7], "fifo") && !ft_strcmp(argv[7], "edf"))
	{
		write_error("[Error] arg is not fifo or edf\n");
		return (0);
	}
	return (1);
}

static void	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_init(&data->dongles[i], NULL);
		i++;
	}
}

static void	parser_init_atoi(t_data *setting, char **argv)
{
	setting->time_to_burnout = atoi(argv[1]);
	setting->time_to_compile = atoi(argv[2]);
	setting->time_to_debug = atoi(argv[3]);
	setting->time_to_refactor = atoi(argv[4]);
	setting->number_of_compiles_required = atoi(argv[5]);
	setting->dongle_cooldown = atoi(argv[6]);
	setting->scheduler = argv[7];
	setting->stop_sim = 0;
}

static int	parsing_malloc(t_data *setting)
{
	setting->dongle_status = malloc(sizeof(int) * setting->number_of_coders);
	if (!setting->dongle_status)
		return (0);
	setting->dongles = malloc(sizeof(pthread_mutex_t)
			* setting->number_of_coders);
	if (!setting->dongles)
		return (0);
	return (1);
}

t_data	*parser(char **argv)
{
	t_data	*setting;

	if (!verif_argv(argv))
		return (0);
	setting = malloc(sizeof(t_data));
	if (!setting)
		return (0);
	setting->queue = NULL;
	setting->number_of_coders = atoi(argv[0]);
	setting->dongle_last_released = malloc(sizeof(long int)
			* setting->number_of_coders);
	if (!setting->dongle_last_released)
		return (0);
	memset(setting->dongle_last_released, 0, sizeof(long int)
		* setting->number_of_coders);
	parser_init_atoi(setting, argv);
	if (!parsing_malloc(setting))
		return (0);
	init_dongles(setting);
	pthread_mutex_init(&setting->queue_mutex, NULL);
	pthread_mutex_init(&setting->stop_mutex, NULL);
	pthread_cond_init(&setting->queue_cond, NULL);
	pthread_mutex_init(&setting->write_mutex, NULL);
	return (setting);
}
