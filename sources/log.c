/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurek <makurek@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:35:52 by makurek           #+#    #+#             */
/*   Updated: 2025/09/23 13:35:54 by makurek          ###   ####lausanne.ch   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long		current_timestamp_ms(void);
static int	ft_strcmp(const char *s1, const char *s2);

int	log_and_sleep(const char *action, t_philo *philo, int duration)
{
	long	timestamp;
	long	end_time;
	int		can_die;

	can_die = ft_strcmp(action, "is eating");
	if (can_die && check_dead(philo))
		return (1);
	timestamp = current_timestamp_ms() - philo->start_time;
	printf("%li %i %s\n", timestamp, philo->id, action);
	end_time = current_timestamp_ms() + duration;
	while (current_timestamp_ms() < end_time)
	{
		usleep(1000);
		if (can_die && check_dead(philo))
			return (1);
	}
	return (0);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

long	current_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}
