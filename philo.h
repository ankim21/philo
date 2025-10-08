/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:25:52 by ankim             #+#    #+#             */
/*   Updated: 2025/10/07 18:19:52 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_table	t_table;
typedef struct s_philo	t_philo;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	mutex;
	long			meal_counter;
	bool			full;
	long			last_meal_time;
	int				fork_id_right;
	int				fork_id_left;
	pthread_t		thread_id;
	t_table			*table;
}	t_philo;

struct s_table
{
	long			philo_nbr;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			nbr_of_meals;
	long			start_simulation;
	bool			end_simulation;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	pthread_t		monitor_thread;
	t_philo			*philos;
};

int		ft_atoi(const char *nptr);
int		param_check(int ac, char **av);
int		init_table_parsing(t_table *table, int ac, char **av);
int		all_array_memory(t_table *table);
int		init_mutex(t_table *table);
void	init_philos(t_table *table);
t_table	*init_table(t_table *table, int ac, char **av);
int		create_philo_thread(t_table *table);
long	gettimems(void);
long	gettimems_action(t_philo *philo);
void	log_state(t_philo *philo, char *action);
int		join_threads(t_table *table);
void	*routine(void *arg);
void	*death_monitor(void *arg);
int		take_forks(t_philo *philo);
int		eat(t_philo *philo);
int		put_forks_down(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		philo_think(t_philo *philo);
int		all_eaten(t_table *table);
int		starved_philo(void *arg);
void	clean_up(t_table *table);
void	free_me(t_table *table);
bool	check_simulation_end(t_table *table);
bool	check_philo_full(t_philo *philo);

#endif