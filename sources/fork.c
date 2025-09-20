#include "philosophers.h"

static int	try_take_fork(t_philo *philo, int fork_index, int *acquired);

int	attempt_fork_acquisition(t_philo *philo, int first, int second)
{
	int	first_acquired;
	int	second_acquired;

	first_acquired = 0;
	second_acquired = 0;
	while (!(first_acquired && second_acquired))
	{
		if (check_dead(philo))
			return (1);
		if (!first_acquired)
			try_take_fork(philo, first, &first_acquired);
		if (!second_acquired)
			try_take_fork(philo, second, &second_acquired);
		usleep(1000);
	}
	return (0);
}

static int	try_take_fork(t_philo *philo, int fork_index, int *acquired)
{
	pthread_mutex_lock(&philo->forks[fork_index].mutex);
	if (philo->forks[fork_index].state == 0)
	{
		philo->forks[fork_index].state = 1;
		*acquired = 1;
	}
	pthread_mutex_unlock(&philo->forks[fork_index].mutex);
	return (*acquired);
}

void	release_forks(t_philo *philo, int first, int second)
{
	pthread_mutex_lock(&philo->forks[first].mutex);
	philo->forks[first].state = 0;
	pthread_mutex_unlock(&philo->forks[first].mutex);
	pthread_mutex_lock(&philo->forks[second].mutex);
	philo->forks[second].state = 0;
	pthread_mutex_unlock(&philo->forks[second].mutex);
}
