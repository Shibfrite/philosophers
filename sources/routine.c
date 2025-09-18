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
	eat_goal = 0;
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
	/*if (philo->args[0] == 1)
	{
		printf("bro took the only fork and left\n");
		return (1);
	}*/
	if (attempt_fork_acquisition(philo, first_fork, second_fork))
		return (1);
	pthread_mutex_lock(&philo->mutexes[MUTEX_MEAL]);
	philo->last_meal = current_timestamp_ms();
	pthread_mutex_unlock(&philo->mutexes[MUTEX_MEAL]);
	//printf("%li\n", philo->last_meal - philo->start_time);
	if(log_and_sleep("is eating", philo, philo->args[TIMER_EATING]))
		return (1);
	release_forks(philo, first_fork, second_fork);
	if ((philo->id % 2) && log_and_sleep("is sleeping", philo, philo->args[TIMER_SLEEPING]))
		return (1);
	return (0);
}

static int attempt_fork_acquisition(t_philo *philo, int first, int second)
{
    int first_acquired = 0;
    int second_acquired = 0;

    while (!first_acquired || !second_acquired)
    {
        if (check_dead(philo, "waiting for forks"))
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

    printf("philo %d took forks: %d %d\n", philo->id, first, second);
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
	printf("At %li philo %d released forks: %d %d\n", current_timestamp_ms() - philo->start_time, philo->id, first, second);
}

int check_dead(t_philo *philo, char *context)
{
	long	since_meal;
	int		dead_status;

	pthread_mutex_lock(philo->dead_mutex);
	dead_status = *philo->dead;
	if (dead_status)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return 1;
	}
	pthread_mutex_lock(&philo->mutexes[MUTEX_MEAL]);
	since_meal = current_timestamp_ms() - philo->last_meal;
	pthread_mutex_unlock(&philo->mutexes[MUTEX_MEAL]);
	if (since_meal >= philo->args[TIMER_DEAD])
	{
		(void)context;
		//printf("flag: %i\n", *philo->dead);
		//printf("philo %d died while %s\n", philo->id, context);
		*philo->dead = 1;
		pthread_mutex_unlock(philo->dead_mutex);
		printf("%li %i died\n",
			current_timestamp_ms() - philo->start_time,
			philo->id);
		return 1;
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return 0;
}

/*
static int attempt_fork_acquisition(t_philoeters *p, int first, int second)
{
	int	 local_dead;

	while (1)
	{
		pthread_mutex_lock(p->mutexes[MUTEX_DEAD]);
		local_dead = *p->dead;
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
		local_dead = *p->dead;
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
