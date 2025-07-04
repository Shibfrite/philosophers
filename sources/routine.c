#include "philosophers.h"

void	print_death(long timestamp, int id)
{
	ft_putnbr_fd(timestamp, 1);
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	write(1, " died\n", 6);
}

int	 check_death(t_parameters *param)
{
	long now = current_timestamp_ms();

	pthread_mutex_lock(param->mutexes[MUTEX_DEATH]);
	pthread_mutex_lock(param->mutexes[MUTEX_MEAL]);
	if (!(*param->death_flag) &&
		(now - *param->last_meal > param->timers[TIMER_DEATH]))
	{
		*param->death_flag = 1;
		pthread_mutex_unlock(param->mutexes[MUTEX_MEAL]);
		pthread_mutex_lock(param->mutexes[MUTEX_PRINT]);
		print_death(now - param->start_time, param->philo_id + 1);
		pthread_mutex_unlock(param->mutexes[MUTEX_PRINT]);
	}
	else
		pthread_mutex_unlock(param->mutexes[MUTEX_MEAL]);
	pthread_mutex_unlock(param->mutexes[MUTEX_DEATH]);
	return (*param->death_flag);
}

static int attempt_fork_acquisition(t_parameters *p, int first, int second)
{
	int	 local_death;

	while (1)
	{
		pthread_mutex_lock(p->mutexes[MUTEX_DEATH]);
		local_death = *p->death_flag;
		pthread_mutex_unlock(p->mutexes[MUTEX_DEATH]);
		if (check_death(p) || local_death)
			return (0);
		pthread_mutex_lock(&p->forks[first]);
		if (!p->fork_state[first])
		{
			pthread_mutex_lock(&p->forks[second]);
			if (!p->fork_state[second])
			{
				p->fork_state[first] = 1;
				pthread_mutex_unlock(&p->forks[first]);
				p->fork_state[second] = 1;
				pthread_mutex_unlock(&p->forks[second]);
				return (1);
			}
			pthread_mutex_unlock(&p->forks[second]);
		}
		pthread_mutex_unlock(&p->forks[first]);
		usleep(1000);
	}
}

static void release_forks(t_parameters *p)
{
	int id = p->philo_id;
	int right = id;
	int left = (id + 1) % 5;

	pthread_mutex_lock(&p->forks[right]);
	p->fork_state[right] = 0;
	pthread_mutex_unlock(&p->forks[right]);

	pthread_mutex_lock(&p->forks[left]);
	p->fork_state[left] = 0;
	pthread_mutex_unlock(&p->forks[left]);
}

void *routine(void *arg)
{
	t_parameters *p = (t_parameters *)arg;
	int	 local_death;

	while (1)
	{
		pthread_mutex_lock(p->mutexes[MUTEX_DEATH]);
		local_death = *p->death_flag;
		pthread_mutex_unlock(p->mutexes[MUTEX_DEATH]);
		if (check_death(p) || local_death)
			break ;
		log_and_sleep("is thinking", p, 0);
		if (!attempt_fork_acquisition(p, (p->philo_id + p->philo_id % 2) % 5,
			(p->philo_id + 1 - p->philo_id % 2) % 5))
			break ;
		pthread_mutex_lock(p->mutexes[MUTEX_MEAL]);
		*(p->last_meal) = current_timestamp_ms();
		pthread_mutex_unlock(p->mutexes[MUTEX_MEAL]);
		log_and_sleep("is eating", p, p->timers[TIMER_EATING]);
		release_forks(p);
		log_and_sleep("is sleeping", p, p->timers[TIMER_SLEEPING]);
	}
	return (NULL);
}
