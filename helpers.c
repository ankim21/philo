/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:24:01 by ankim             #+#    #+#             */
/*   Updated: 2025/10/07 18:20:50 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	sign = 1;
	res = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (res * sign);
}

int	param_check(int ac, char **av)
{
	int	i;
	int	res;

	i = 1;
	if (ac != 6 && ac != 5)
	{
		printf("Invalid!\n");
		return (-1);
	}
	while (i < ac)
	{
		res = ft_atoi(av[i]);
		if (ac == 6 && (i == (ac - 1)))
		{
			if (res < 0)
			{
				printf("Invalid!\n");
				return (-1);
			}
		}
		i++;
	}
	return (0);
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

bool	check_simulation_end(t_table *table)
{
	bool	sim_end;

	pthread_mutex_lock(&table->end_mutex);
	sim_end = table->end_simulation;
	pthread_mutex_unlock(&table->end_mutex);
	return (sim_end);
}

bool	check_philo_full(t_philo *philo)
{
	bool	is_full;

	pthread_mutex_lock(&philo->mutex);
	is_full = philo->full;
	pthread_mutex_unlock(&philo->mutex);
	return (is_full);
}
