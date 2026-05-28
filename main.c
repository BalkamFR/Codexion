/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:55:42 by papilaz           #+#    #+#             */
/*   Updated: 2026/05/27 14:24:10 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("%d is compiling\n", coder->id);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data *setting;
	t_coder *coder;
	int i;

	i = 0;

	setting = parser(argv + 1);
	if (!setting)
		return (1);

	coder = malloc(sizeof(t_coder) * setting->number_of_coders);
	if (!coder)
		return (1);
	init_all_coder(setting, &coder);
	while (i < setting->number_of_coders)
	{
		pthread_create(&coder[i].thread, NULL, routine, &coder[i]);
		i++;
	}
	join_all_coder(setting, coder);
	(void)argc;
	(void)argv;
	return (0);
}