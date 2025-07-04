#include "philosophers.h"

long	current_timestamp_ms(void)
{
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	ft_putnbr_fd(long n, int fd)
{
	char	c;

	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = '0' + (n % 10);
	write(fd, &c, 1);
}

size_t  ft_strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return (p - s);
}


void	fast_log(const char *action, t_parameters *param)
{
	long	timestamp = current_timestamp_ms() - param->start_time;

	pthread_mutex_lock(param->mutexes[MUTEX_PRINT]);
	pthread_mutex_lock(param->mutexes[MUTEX_DEATH]);
	if (!(*param->death_flag))
	{
		ft_putnbr_fd(timestamp, 1);
		write(1, " ", 1);
		ft_putnbr_fd(param->philo_id + 1, 1);
		write(1, " ", 1);
		write(1, action, ft_strlen(action));
		write(1, "\n", 1);
	}
	pthread_mutex_unlock(param->mutexes[MUTEX_DEATH]);
	pthread_mutex_unlock(param->mutexes[MUTEX_PRINT]);
}

void	log_and_sleep(const char *action, t_parameters *param, int duration)
{
	long	start;
	long	is_death;

	fast_log(action, param);
	start = current_timestamp_ms();
	while (current_timestamp_ms() - start < duration)
	{
		pthread_mutex_lock(param->mutexes[MUTEX_DEATH]);
		is_death = *param->death_flag;
		pthread_mutex_unlock(param->mutexes[MUTEX_DEATH]);
		if (is_death || check_death(param))
			return;
		usleep(100);
	}
}
