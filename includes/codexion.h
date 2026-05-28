#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
typedef struct s_node
{
	struct s_coder	*coder;
	struct s_node	*next;
}					t_node;

typedef struct s_data
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	write_mutex;
	t_node			*queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
	long int		start_time;
	int				stop_sim;
	pthread_mutex_t	stop_mutex;
	int				*dongle_status;
}					t_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_data			*data;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
}					t_coder;


t_data				*parser(char **argv);
void				write_error(char *tab);
int					ft_strlen(char *tab);
int					ft_is_number(char number);
int					init_all_coder(t_data *setting, t_coder **coder);
void join_all_coder(t_data *setting, t_coder *coder);
#endif