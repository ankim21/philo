/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overall_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:08:33 by ankim             #+#    #+#             */
/*   Updated: 2025/10/07 21:15:25 by ankim            ###   ########.fr       */
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
		usleep(200);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if ((philo->id) % 2 == 0)
	{
		usleep(philo->table->time_to_eat * 1000);
		log_state(philo, "is thinking");
	}
	while (1)
	{
		if (check_simulation_end(philo->table))
			break ;
		if (check_philo_full(philo))
			break ;
		eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

int	all_eaten(t_table *table)
{
	int		i;

	i = 0;
	if (!table)
		return (-1);
	while (i < table->philo_nbr)
	{
		if (!check_philo_full(&table->philos[i]))
			return (0);
		i++;
	}
	pthread_mutex_lock(&table->end_mutex);
	table->end_simulation = true;
	pthread_mutex_unlock(&table->end_mutex);
	return (1);
}

int	starved_philo(void *arg)
{
	t_table	*table;
	long	time;
	int		i;

	table = (t_table *)arg;
	i = 0;
	while (i < table->philo_nbr)
	{
		if (check_simulation_end(table))
			return (0);
		pthread_mutex_lock(&table->philos[i].mutex);
		time = gettimems() - table->philos[i].last_meal_time;
		if ((!table->philos[i].full) && (time > table->time_to_die))
		{
			log_state(&table->philos[i], "died");
			pthread_mutex_unlock(&table->philos[i].mutex);
			pthread_mutex_lock(&table->end_mutex);
			table->end_simulation = true;
			pthread_mutex_unlock(&table->end_mutex);
			return (1);
		}
		pthread_mutex_unlock(&table->philos[i].mutex);
		i++;
	}
	return (0);
}

void	clean_up(t_table *table)
{
	if (!table)
		return ;
	free_me(table);
}
