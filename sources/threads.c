/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurek <makurek@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:36:01 by makurek           #+#    #+#             */
/*   Updated: 2025/09/23 13:36:03 by makurek          ###   ####lausanne.ch   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		init_fork(t_fork *fork);
static t_philo	init_philo(const int *args, long start, int i, int *dead_flag);
static void		create_philo(t_context *ctx);

int	manage_threads_and_mutexes(const int *args)
{
	t_context	ctx;
	int			i;

	ctx.args = args;
	i = 0;
	while (i < args[0])
		init_fork(&ctx.forks[i++]);
	pthread_mutex_init(&ctx.dead_mutex, NULL);
	create_philo(&ctx);
	i = 0;
	while (i < args[0])
		pthread_join(ctx.thread_id[i++], NULL);
	return (0);
}

static void	create_philo(t_context *ctx)
{
	int		i;
	long	start;

	start = current_timestamp_ms();
	ctx->dead = 0;
	i = 0;
	while (i < ctx->args[0])
	{
		ctx->philo[i] = init_philo(ctx->args, start, i, &ctx->dead);
		ctx->philo[i].dead_mutex = &ctx->dead_mutex;
		ctx->philo[i].forks = ctx->forks;
		if (ctx->args[4])
			ctx->philo[i].eat_goal = ctx->args[4];
		else
			ctx->philo[i].eat_goal = -1;
		pthread_create(&ctx->thread_id[i], NULL, routine, &ctx->philo[i]);
		i++;
	}
}

static t_philo	init_philo(const int *args, long start, int i, int *dead_flag)
{
	t_philo	philo;

	philo.id = i;
	philo.start_time = start;
	philo.last_meal = start;
	philo.args = args;
	pthread_mutex_init(&philo.mutexes[MUTEX_MEAL], NULL);
	philo.dead = dead_flag;
	return (philo);
}

static void	init_fork(t_fork *fork)
{
	fork->state = 0;
	pthread_mutex_init(&fork->mutex, NULL);
}
