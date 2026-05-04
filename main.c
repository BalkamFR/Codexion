/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: papilaz <papilaz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 19:55:42 by papilaz           #+#    #+#             */
/*   Updated: 2026/05/04 01:57:35 by papilaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


int main(int argc, char **argv)
{
	t_list *setting;
	
	setting = parser(argv + 1);
	if (!setting)
		return (1);
	(void)argc;
	printf("%d", setting->number_of_coders);
	// free(setting);
	return 0;
}