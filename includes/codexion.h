#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/time.h>
#include <string.h>
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
	char				*scheduler;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	write_mutex;
	t_node			*queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
	long int		start_time;
	int				stop_sim;
	pthread_mutex_t	stop_mutex;
	int				*dongle_status;
    long int *dongle_last_released;
}					t_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_data			*data;
	int			compile_count;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	long int    last_compile_start;
}					t_coder;


t_data				*parser(char **argv);
void				write_error(char *tab);
int					ft_strlen(char *tab);
int					ft_is_number(char number);
int					init_all_coder(t_data *setting, t_coder **coder);
int create_all_coder(t_coder *coder, t_data *setting, void *(*routine)(void *));
void join_all_coder(t_data *setting, t_coder *coder);
int	push_to_queue(t_data *data, t_coder *coder);
int 	pop_queue(t_data *data);
long int get_time(void);
void    print_status(t_coder *coder, char *status);
void    *monitor(void *arg);
int	verif_scheduler(char *tab, char *tab_2);
#endif