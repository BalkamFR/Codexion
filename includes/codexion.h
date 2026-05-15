#ifndef HEADER_H
# define HEADER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct s_list
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
}					t_data;

typedef struct  s_coder
{
	int id;
	pthread_t thread;
	t_data data;
}			t_coder;


t_data *parser(char **argv);
void	write_error(char *tab);
int		ft_strlen(char *tab);
int ft_is_number(char number);
int init_all_coder(t_data *setting, t_coder **coder);
#endif