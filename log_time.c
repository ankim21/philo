/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:01:51 by ankim             #+#    #+#             */
/*   Updated: 2025/06/15 16:52:49 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettimems(void)
{
	struct timeval tv;
	long	current_ms;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	current_ms = tv.tv_sec * 1000 + tv.tv_usec/ 1000;
	return (current_ms);
}

long	gettimems_action(t_philo *philo)
{
	long	start;
	long	current;

	if (!philo)
		return(-1);
	start = philo->table->start_simulation;
	current = gettimems() - start;
	return(current);
}

void	log_state(t_philo *philo, char *action)
{
	if (!philo || !philo->table)
		return;
	if (philo->table->end_simulation)
		return;
	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ld %d %s\n", gettimems_action(philo), philo->id, action);
	pthread_mutex_unlock(&philo->table->print_mutex);
}
