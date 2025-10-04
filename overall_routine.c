/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overall_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:08:33 by ankim             #+#    #+#             */
/*   Updated: 2025/10/04 13:32:38 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*death_monitor(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->end_mutex);
		if (table->end_simulation)
		{
			pthread_mutex_unlock(&table->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->end_mutex);
		if (starved_philo(table) == 1)
			break ;
		if (all_eaten(table) == 1)
		{
			printf("FULL!\n");
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo*) arg;
	if ((philo->id) % 2 == 0)
		usleep(1000);
	while (!philo->table->end_simulation)
	{
		eat(philo);
		sleep_think(philo);
	}
	return (NULL);
}

int	all_eaten(t_table *table)
{
	int		i;
	bool	is_full;

	i = 0;
	if (!table)
		return (-1);
	while (i < table->philo_nbr)
	{
		pthread_mutex_lock(&table->philos[i].mutex);
		is_full = table->philos[i].full;
		pthread_mutex_unlock(&table->philos[i].mutex);
		if (!is_full)
			return(0);
		i++;
	}
	pthread_mutex_lock(&table->end_mutex);
	table->end_simulation = true;
	pthread_mutex_unlock(&table->end_mutex);
	return(1);
}

int	starved_philo(void *arg)
{
	t_table *table;
	long	time;
	int		i;
	bool	is_full;

	i = 0;
	table = (t_table *)arg;
	while (!table->end_simulation && i < table->philo_nbr)
	{
		pthread_mutex_lock(&table->philos[i].mutex);
		time = gettimems() - table->philos[i].last_meal_time;
		is_full = table->philos[i].full;
		if ((!is_full) && (time > table->time_to_die))
		{
			log_state(&table->philos[i], "died");
			pthread_mutex_lock(&table->end_mutex);
			table->end_simulation = true;
			pthread_mutex_unlock(&table->end_mutex);
			return (1);
		}
		pthread_mutex_unlock(&table->philos[i].mutex);
		i++;
	}
	return(0);
}

void	clean_up(t_table *table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	while (i < table->philo_nbr)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;		
	}
	pthread_join(table->monitor_thread, NULL);
	free_me(table);
}

void	free_me(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	i = 0;
	while (i < table->philo_nbr)
	{
		pthread_mutex_destroy(&table->philos[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&table->end_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	free(table->philos);
	free(table->forks);
	free(table);
}
