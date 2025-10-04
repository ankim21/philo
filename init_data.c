/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:04:18 by ankim             #+#    #+#             */
/*   Updated: 2025/10/04 13:27:00 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

int	init_table_parsing(t_table *table, int ac, char **av)
{
	if (!table)
		return (-1);
	table->philo_nbr = ft_atoi(av[1]);
	if (table->philo_nbr == 1)
	{
		printf("Error: Cannot have only one philo..\n");
		return (-1);
	}
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->nbr_of_meals = ft_atoi(av[5]);
	else if (ac == 5)
	{
		table->nbr_of_meals = -1;
		printf("Unspecified number of meals\n");
	}
	table->end_simulation = false;
	return (0);
}

int	all_array_memory(t_table *table)
{
	if (!table)
		return (-1);
	table->philos = malloc(sizeof(t_philo) * table->philo_nbr);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_nbr);
	if (!(table->forks) || !(table->philos))
		return (-1);
	return (0);
}

int	init_mutex(t_table *table)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&table->end_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (-1);
	while (i < table->philo_nbr)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (-1);
		if (pthread_mutex_init(&table->philos[i].mutex, NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}

void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	while (i < table->philo_nbr)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meal_counter = 0;
		table->philos[i].full = false;
		table->philos[i].last_meal_time = 0;
		table->philos[i].fork_id_left = i;
		table->philos[i].fork_id_right = (i + 1) % table->philo_nbr;
		table->philos[i].table = table;
		i++;
	}
}

t_table	*init_table(t_table *table, int ac, char **av)
{
	if (!table)
		return (NULL);
	if (init_table_parsing(table, ac, av) != 0)
		return (NULL);
	if (all_array_memory(table) != 0)
		return (NULL);
	if (init_mutex(table) != 0)
		return (NULL);
	init_philos(table);
	return (table);
}
