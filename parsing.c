/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:48:48 by papilaz           #+#    #+#             */
/*   Updated: 2026/05/04 01:57:13 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	verif_argv(char **argv)
{
	int i;
	int a;

	i = 0;
	a = 0;
	while (argv[i])
	{
		while (argv[i][a])
		{
			if (!ft_is_number(argv[i][a]) || atoi(argv[i]) == 0)
			{
				write_error("[Error] negative value or str is forbiden\n");	
				return (0);
			}
			a++;
		}
		a = 0;
		i++;
	}
	return (1);
}

t_list *parser(char **argv)
{
	t_list *setting;

	(void)argv;
	if (!verif_argv(argv))
		return 0;
	setting = malloc(sizeof(t_list));
	if (!setting)
		return 0;
	setting->number_of_coders = atoi(argv[0]);
	setting ->  time_to_burnout = atoi(argv[1]);
	setting -> time_to_compile = atoi(argv[2]);
	setting -> time_to_debug = atoi(argv[3]);
	setting -> time_to_refactor = atoi(argv[4]);
	setting -> number_of_compiles_required = atoi(argv[5]);
	// setting -> dongle_cooldown = atoi(argv[6]);
	// setting -> scheduler = atoi(argv[7]);
	
	return(setting);
}
