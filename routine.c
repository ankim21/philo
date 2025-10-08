/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankim <ankim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:17:20 by ankim             #+#    #+#             */
/*   Updated: 2025/10/07 18:39:09 by ankim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	if (!philo)
		return (-1);
	if ((philo->id) % 2 == 0)
	{
		usleep(250);
		pthread_mutex_lock(&philo->table->forks[philo->fork_id_left]);
		log_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[philo->fork_id_right]);
		log_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->table->forks[philo->fork_id_right]);
		log_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->forks[philo->fork_id_left]);
		log_state(philo, "has taken a fork");
	}
	return (0);
}

int	put_forks_down(t_philo *philo)
{
	if (!philo)
		return (-1);
	if ((philo->id) % 2 == 0)
	{
		pthread_mutex_unlock(&philo->table->forks[philo->fork_id_right]);
		pthread_mutex_unlock(&philo->table->forks[philo->fork_id_left]);
	}
	else
	{
		pthread_mutex_unlock(&philo->table->forks[philo->fork_id_left]);
		pthread_mutex_unlock(&philo->table->forks[philo->fork_id_right]);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	if (!philo)
		return (-1);
	take_forks(philo);
	log_state(philo, "is eating");
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal_time = gettimems();
	philo->meal_counter++;
	if (philo->table->nbr_of_meals != -1
		&& (philo->meal_counter >= philo->table->nbr_of_meals))
			philo->full = true;
	pthread_mutex_unlock(&philo->mutex);
	usleep((philo->table->time_to_eat) * 1000);
	put_forks_down(philo);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	log_state(philo, "is sleeping");
	usleep(philo->table->time_to_sleep * 1000);
	return (0);
}

int	philo_think(t_philo *philo)
{
	long	thinktime;
	long	think_duration;

	thinktime = philo->table->time_to_die
		- philo->table->time_to_eat - philo->table->time_to_sleep;
	if (thinktime > 0)
	{
		log_state(philo, "is thinking");
		if (philo->table->philo_nbr % 2 == 1)
			think_duration = thinktime * 200;
		else
			think_duration = thinktime * 800;
		usleep(think_duration);
	}
	return (0);
}
