#include "philosophers.h"

static int	parsing(const char **args, int *args_new);
static int	manage_threads_and_mutexes(const int *args);

int	main(int ac, const char **args)
{
	int	args_num[6];

	if (ac != 5 && ac != 6)
		return (write(2, "arg error\n", 10), 1);
	if (parsing(args, args_num))
		return (1);
	if (!args_num[0])
		return (0);
	if (manage_threads_and_mutexes((const int *)args_num))
		return (1);
	return (0);
}


//--
static void	init_fork(t_fork *fork);
static t_philo	init_philo(const int *args, long start, int i, int *dead_flag);

static int  manage_threads_and_mutexes(const int *args)
{
	t_fork			forks[MAX_PHILO];
	t_philo			philo[MAX_PHILO];
	pthread_t		thread_id[MAX_PHILO];
	long			start;
	int				i;
	int				dead;
	pthread_mutex_t dead_mutex;

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
		philo[i].dead_mutex = &dead_mutex; // share one mutex
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
	return philo;
}


static void	init_fork(t_fork *fork)
{
	fork->state = 0;
	pthread_mutex_init(&fork->mutex, NULL);
}

/*
int main(int ac, char **av)
{
	pthread_mutex_t fork[MAX_PHILO], print, dead, meal[MAX_PHILO];
	pthread_t	  philo[MAX_PHILO];
	t_parameters	param[MAX_PHILO];
	long			last_meal[MAX_PHILO];
	int				 philo_count = 5;
	int				 i;
	int				 dead_flag = 0;
	long			start = current_timestamp_ms();

	if (ac != 4)
		return (0);
	pthread_mutex_init(&print, NULL);
	pthread_mutex_init(&dead, NULL);
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
		param[i].dead_flag = &dead_flag;
		param[i].last_meal = &last_meal[i];
		init_mutexes(&print, &dead, &meal[i], param[i].mutexes);
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
	pthread_mutex_destroy(&dead);
	return (0);
}*/

static int	ft_atoi(const char *str);
static int	parsing(const char **args, int *args_num)
{
	int	*start;

	if (!args || !args_num)
		return (1);
	start = args_num;
	while(*++args)
	{
		*args_num = ft_atoi(*args);
		if (*args_num == -1)
			return (printf("non-numerical arg %s\n", *args), 1);
		args_num++;
	}
	if (!start[0])
		return (write(2, "no philosopher\n", 15), 1);
	if (start[0] > MAX_PHILO)
		return (write(2, "too many philosophers\n", 22), 1);
	args_num = start;
	return (0);
}

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	if (*str)
		return (-1);
	return (result * sign);
}
/*
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
}*/
