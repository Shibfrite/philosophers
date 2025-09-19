#include "philosophers.h"

static void		init_fork(t_fork *fork);
static t_philo	init_philo(const int *args, long start, int i, int *dead_flag);

int  manage_threads_and_mutexes(const int *args)
{
	pthread_mutex_t dead_mutex;
	pthread_t		thread_id[MAX_PHILO];
	t_philo			philo[MAX_PHILO];
	t_fork			forks[MAX_PHILO];
	long			start;
	int				dead;
	int				i;

	start = current_timestamp_ms();
	dead = 0;
	pthread_mutex_init(&dead_mutex, NULL);
	i = 0;
	init_fork(&forks[i]);
	while (i < args[0])
	{
		if (i + 1 <= args[0])
			init_fork(&forks[i + 1]);
		philo[i] = init_philo(args, start, i, &dead);
		philo[i].forks = forks;
		philo[i].dead_mutex = &dead_mutex;
		if (args[4])
			philo[i].eat_goal = args[4];
		else	
			philo[i].eat_goal = -1;
		pthread_create(&thread_id[i], NULL, routine, &philo[i]);
		++i;
	}
	while (i--)
		pthread_join(thread_id[i], NULL);
	return (0);
}

static t_philo init_philo(const int *args, long start, int i, int *dead_flag)
{
	t_philo philo;

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
