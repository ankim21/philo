/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:23:30 by ankim             #+#    #+#             */
/*   Updated: 2025/10/07 21:04:53 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo_thread(t_table *table)
{
	int	i;

	i = 0;
	if (!table)
		return (-1);
	while (i < table->philo_nbr)
	{
		if (pthread_create(&table->philos[i].thread_id, NULL,
				routine, &table->philos[i]) != 0)
		{
			while (i >= 0)
			{
				pthread_join(table->philos[i].thread_id, NULL);
				i--;				
			}
			printf("Failed to create threads\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		if (pthread_join(table->philos[i].thread_id, NULL) != 0)
		{
			printf ("Failed to join thread");
			return (-1);
		}
		i++;
	}
	if (pthread_join(table->monitor_thread, NULL) != 0)
	{
		printf("Failed to join death thread");
		return (-1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*table;
	int		i;

	i = 0;
	if (param_check(ac, av) == -1)
		return (0);
	table = malloc(sizeof(t_table));
	if (!table)
		return (1);
	if (init_table(table, ac, av) == NULL)
		return (free(table), 1);
	table->start_simulation = gettimems();
	while (i < table->philo_nbr)
	{
		pthread_mutex_lock(&table->philos[i].mutex);
		table->philos[i].last_meal_time = table->start_simulation;
		pthread_mutex_unlock(&table->philos[i].mutex);
		i++;
	}
	if (create_philo_thread(table) != 0)
		return (free_me(table), 1);
	pthread_create(&table->monitor_thread, NULL, death_monitor, table);
	join_threads(table);
	return (free_me(table), 0);
}
