/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:02:34 by papilaz           #+#    #+#             */
/*   Updated: 2026/06/12 23:15:09 by papilaz          ###   ########.fr       */
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

long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_strcmp(char *tab, char *tab_2)
{
	int	i;

	i = 0;
	while (tab[i] || tab_2[i])
	{
		if (tab[i] != tab_2[i])
			return (0);
		i++;
	}
	return (1);
}
