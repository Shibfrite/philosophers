#include "philosophers.h"

static int	attempt_fork_acquisition(t_philo *philo, int first, int second);
static void	release_forks(t_philo *philo, int first, int second);

void    *routine(void *args)
{
    t_philo *philo;
    int     id;
    int     first_fork;
    int     second_fork;

    philo = (t_philo *)args;
    id = philo->id;
    first_fork = (id + id % 2) % philo->args[0];
    second_fork = (id + 1 - id % 2) % philo->args[0];
    printf("in routine\n");
    while (1)
        if (philo_cycle(philo, first_fork, second_fork))
            break ;
    return (NULL);
}

static int  philo_cycle(t_philo *philo, int first_fork, int second_fork)
{
    log_and_sleep("is thinking", philo, 0);
    /*if (philo->args[0] == 1)
    {
        printf("bro took the only fork and left\n");
        return (1);
    }*/
    if (attempt_fork_acquisition(philo, first_fork, second_fork))
        return (1);
    log_and_sleep("is eating", philo, philo->args[TIMER_EATING]);
    release_forks(philo, first_fork, second_fork);
    pthread_mutex_lock(philo->mutexes[MUTEX_MEAL]);
    printf("last meal: %li\n", current_timestamp_ms() - philo->last_meal);
    philo->last_meal = current_timestamp_ms();
    pthread_mutex_unlock(philo->mutexes[MUTEX_MEAL]);
    log_and_sleep("is sleeping", philo, philo->args[TIMER_SLEEPING]);
    return (0);
}

static int attempt_fork_acquisition(t_philo *philo, int first, int second)
{
	while (1)
	{
		pthread_mutex_lock(&philo->forks[first]);
		if (!philo->fork_state[first])
		{
			pthread_mutex_lock(&philo->forks[second]);
			if (!philo->fork_state[second])
			{
				philo->fork_state[first] = 1;
				pthread_mutex_unlock(&philo->forks[first]);
				philo->fork_state[second] = 1;
				pthread_mutex_unlock(&philo->forks[second]);
				return (0);
			}
			pthread_mutex_unlock(&philo->forks[second]);
		}
		pthread_mutex_unlock(&philo->forks[first]);
		usleep(1000);
	}
}

static void release_forks(t_philo *philo, int first, int second)
{
	pthread_mutex_lock(&philo->forks[first]);
	philo->fork_state[first] = 0;
	pthread_mutex_unlock(&philo->forks[first]);

	pthread_mutex_lock(&philo->forks[second]);
	philo->fork_state[second] = 0;
	pthread_mutex_unlock(&philo->forks[second]);
}

void	print_dead(long timestamp, int id)
{
	ft_putnbr_fd(timestamp, 1);
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	write(1, " died\n", 6);
}

int	 check_dead(t_philo *philo)
{
	long	now;

	pthread_mutex_lock(philo->mutexes[MUTEX_DEAD]);
	if (*philo->dead)
		return (pthread_mutex_unlock(philo->mutexes[MUTEX_DEAD]), 1);
	pthread_mutex_lock(philo->mutexes[MUTEX_MEAL]);
	now = current_timestamp_ms();
	if (!(*philo->dead_flag) &&
		(now - *philo->last_meal > philo->args[TIMER_DEAD]))
	{
		*philo->dead_flag = 1;
		pthread_mutex_unlock(philo->mutexes[MUTEX_MEAL]);
		print_dead(now - philo->start_time, philo->id + 1);
	}
	else
		pthread_mutex_unlock(philo->mutexes[MUTEX_MEAL]);
	pthread_mutex_unlock(philo->mutexes[MUTEX_DEAD]);
	return (*philo->dead_flag);
}
/*
static int attempt_fork_acquisition(t_philoeters *p, int first, int second)
{
	int	 local_dead;

	while (1)
	{
		pthread_mutex_lock(p->mutexes[MUTEX_DEAD]);
		local_dead = *p->dead_flag;
		pthread_mutex_unlock(p->mutexes[MUTEX_DEAD]);
		if (check_dead(p) || local_dead)
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

static void release_forks(t_philoeters *p)
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
	t_philoeters *p = (t_philoeters *)arg;
	int	 local_dead;

	while (1)
	{
		pthread_mutex_lock(p->mutexes[MUTEX_DEAD]);
		local_dead = *p->dead_flag;
		pthread_mutex_unlock(p->mutexes[MUTEX_DEAD]);
		if (check_dead(p) || local_dead)
			break ;
		////log_and_sleep("is thinking", p, 0);
		if (!attempt_fork_acquisition(p, (p->philo_id + p->philo_id % 2) % 5,
			(p->philo_id + 1 - p->philo_id % 2) % 5))
			break ;
		pthread_mutex_lock(p->mutexes[MUTEX_MEAL]);
		*(p->last_meal) = current_timestamp_ms();
		pthread_mutex_unlock(p->mutexes[MUTEX_MEAL]);
		//log_and_sleep("is eating", p, p->timers[TIMER_EATING]);
		release_forks(p);
		//log_and_sleep("is sleeping", p, p->timers[TIMER_SLEEPING]);
	}
	return (NULL);
}*/
