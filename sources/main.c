#include "philosophers.h"

static int	parsing(const char **args, int *args_new);
static int	manage_threads_and_mutexes(const int *args);

int	main(int ac, const char **args)
{
	const int	args_num[6];

	if (ac != 5 && ac != 6)
		return (write(2, "arg error\n", 10), 1);
	if (parsing(args, (int *)args_num))
		return (1);
	if (!args_num[0])
		return (0);
	if (manage_threads_and_mutexes(args_num))
		return (1);
	return (0);
}


//--
static void				init_fork(t_fork *fork);
static pthread_t	*start_philo(const int *args, long start, int i, t_fork *forks);

static int	manage_threads_and_mutexes(const int *args)
{
	pthread_t	*ids;
	t_fork		forks[200];
	long		start;
	int			i;

	start = current_timestamp_ms();
	i = 0;
	init_fork(&forks[i]);
	while (i < args[0])
	{
		if (i + 1 <= args[0])
			init_fork(&forks[i + 1]);
		ids = start_philo(args, start, i, forks);
		++i;
	}
	while (i--)
	    pthread_join(ids[i], NULL);
    printf("Main finished\n");
    return (0);
}

static void	init_fork(t_fork *fork)
{
    pthread_mutex_t	mutex;
	
	fork->state = 0;
	pthread_mutex_init(&mutex, NULL);
	fork->mutex = mutex;
}

static pthread_t *start_philo(const int *args, long start, int i, t_fork *forks)
{
	t_philo			philo[200];
    pthread_t		philo_id[200];
    pthread_mutex_t	meal[200];
    pthread_mutex_t	dead[200];
	int				*dead_flag;

	dead_flag = 0;
	philo[i].id = i;
	philo[i].forks = forks;
	philo[i].start_time = start;
	philo[i].args = args;
	pthread_mutex_init(&meal[i], NULL);
	philo[i].mutexes[1] = &meal[i];
	pthread_mutex_init(&dead[i], NULL);
	philo[i].mutexes[1] = &dead[i];
	philo[i].dead = dead_flag;
	pthread_create(&philo_id[i], NULL, routine, &philo[i]);
	return (philo_id);
}

/*
int main(int ac, char **av)
{
	pthread_mutex_t fork[200], print, dead, meal[200];
	pthread_t	  philo[200];
	t_parameters    param[200];
	long		    last_meal[200];
	int			     philo_count = 5;
	int			     i;
	int			     dead_flag = 0;
	long		    start = current_timestamp_ms();

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
