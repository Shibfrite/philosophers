#include "philosophers.h"

static int	attempt_fork_acquisition(t_philo *philo, int index, int second);
static void	release_forks(t_philo *philo, int first, int second);
static int  philo_cycle(t_philo *philo, int first_fork, int second_fork);

void	*routine(void *args)
{
	t_philo *philo;
	int		id;
	int		first_fork;
	int		second_fork;
	int		eat_goal;

	philo = (t_philo *)args;
	id = philo->id;
	first_fork = (id + id % 2) % philo->args[0];
	second_fork = (id + 1 - id % 2) % philo->args[0];
	eat_goal = 1;
	//if (philo->args[0] == 1)
	//	log_and_sleep("is thinking", philo, philo->args[1]);
	while (1)
	{
		if (philo_cycle(philo, first_fork, second_fork))
			break ;
		if (philo->eat_goal >= 0)
		{
			if (eat_goal == philo->eat_goal)
				return (NULL);
			eat_goal++;
		}
	}
	return (NULL);
}

static int  philo_cycle(t_philo *philo, int first_fork, int second_fork)
{
	if (!(philo->id % 2) && log_and_sleep("is sleeping", philo, philo->args[TIMER_SLEEPING]))
		return (1);
	if (log_and_sleep("is thinking", philo, 0))
		return (1);
	if (attempt_fork_acquisition(philo, first_fork, second_fork))
		return (1);
	pthread_mutex_lock(&philo->mutexes[MUTEX_MEAL]);
	philo->last_meal = current_timestamp_ms();
	pthread_mutex_unlock(&philo->mutexes[MUTEX_MEAL]);
	if(log_and_sleep("is eating", philo, philo->args[TIMER_EATING]))
		return (1);
	release_forks(philo, first_fork, second_fork);
	if ((philo->id % 2) && log_and_sleep("is sleeping", philo, philo->args[TIMER_SLEEPING]))
		return (1);
	return (0);
}

static int	attempt_fork_acquisition(t_philo *philo, int first, int second)
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
        {
            pthread_mutex_lock(&philo->forks[first].mutex);
            if (philo->forks[first].state == 0)
            {
                philo->forks[first].state = 1;
                first_acquired = 1;
            }
            pthread_mutex_unlock(&philo->forks[first].mutex);
        }
        if (!second_acquired)
        {
            pthread_mutex_lock(&philo->forks[second].mutex);
            if (philo->forks[second].state == 0)
            {
                philo->forks[second].state = 1;
                second_acquired = 1;
            }
            pthread_mutex_unlock(&philo->forks[second].mutex);
        }
        usleep(1000);
    }
    return (0);
}


static void release_forks(t_philo *philo, int first, int second)
{
	pthread_mutex_lock(&philo->forks[first].mutex);
	philo->forks[first].state = 0;
	pthread_mutex_unlock(&philo->forks[first].mutex);

	pthread_mutex_lock(&philo->forks[second].mutex);
	philo->forks[second].state = 0;
	pthread_mutex_unlock(&philo->forks[second].mutex);
}

int check_dead(t_philo *philo)
{
	long	since_meal;
	int		dead_status;

	pthread_mutex_lock(philo->dead_mutex);
	dead_status = *philo->dead;
	if (dead_status)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_lock(&philo->mutexes[MUTEX_MEAL]);
	since_meal = current_timestamp_ms() - philo->last_meal;
	pthread_mutex_unlock(&philo->mutexes[MUTEX_MEAL]);
	if (since_meal >= philo->args[TIMER_DEAD])
	{
		*philo->dead = 1;
		pthread_mutex_unlock(philo->dead_mutex);
		printf("%li %i died\n",
			current_timestamp_ms() - philo->start_time,
			philo->id);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}
