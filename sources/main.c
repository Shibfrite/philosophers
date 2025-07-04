#include "philosophers.h"

static int	 ft_atoi(const char *str)
{
	int	 result;
	int	 sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

static void	init_timers(char **args, int *timers)
{
	timers[TIMER_DEATH] = ft_atoi(args[1]);
	timers[TIMER_EATING] = ft_atoi(args[2]);
	timers[TIMER_SLEEPING] = ft_atoi(args[3]);
}

static void	init_mutexes(pthread_mutex_t *print, pthread_mutex_t *death,
		pthread_mutex_t *meal, pthread_mutex_t **mutexes)
{
	mutexes[MUTEX_PRINT] = print;
	mutexes[MUTEX_DEATH] = death;
	mutexes[MUTEX_MEAL] = meal;
}

int main(int ac, char **av)
{
	pthread_mutex_t fork[200], print, death, meal[200];
	pthread_t	   philo[200];
	t_parameters	param[200];
	long			last_meal[200];
	int			 	philo_count = 5;
	int				i;
	int				death_flag = 0;
	long			start = current_timestamp_ms();

	if (ac != 4)
		return (0);
	pthread_mutex_init(&print, NULL);
	pthread_mutex_init(&death, NULL);
	i = 0;
	while (i < philo_count)
		pthread_mutex_init(&fork[i++], NULL);
	i = 0;
	while (i < philo_count)
	{
		pthread_mutex_init(&meal[i], NULL);
		param[i].philo_id = i;
		param[i].forks = fork;
		init_timers(av, param[i].timers);
		param[i].start_time = start;
		param[i].death_flag = &death_flag;
		param[i].last_meal = &last_meal[i];
		init_mutexes(&print, &death, &meal[i], param[i].mutexes);
		last_meal[i] = start;
		pthread_create(&philo[i], NULL, routine, &param[i]);
		i++;
	}
	for (i = 0; i < philo_count; i++)
	{
		pthread_join(philo[i], NULL);
		pthread_mutex_destroy(&fork[i]);
		pthread_mutex_destroy(&meal[i]);
	}
	pthread_mutex_destroy(&print);
	pthread_mutex_destroy(&death);
	return (0);
}
