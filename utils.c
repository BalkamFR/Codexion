// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   utils.c                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/05/03 20:58:32 by papilaz           #+#    #+#             */
// /*   Updated: 2026/05/03 21:06:25 by papilaz          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "includes/codexion.h"

int ft_strlen(char *tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return i;	
}

void	write_error(char *tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		write(2, &tab[i], 1);
		i++;
	}
	write(2, "\n", 1);
}
int ft_is_number(char number)
{
	if (number >= '0' && number <= '9')
		return 1;
	return 0;
}