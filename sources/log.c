#include "philosophers.h"

long	current_timestamp_ms(void);
size_t  ft_strlen(const char *s);
void	ft_putnbr_fd(long n, int fd);
//void	fast_log(const char *action, t_philoeters *param);

void	log_and_sleep(const char *action, t_philo *philo, int duration)
{
	long	start;
	long	timestamp;

	timestamp = current_timestamp_ms() - philo->start_time;
	printf("%li %i %s\n", timestamp, philo->id, action);
	start = current_timestamp_ms();
	while (current_timestamp_ms() - start < duration)
		usleep(100);
}
/*
void	log_and_sleep(const char *action, t_philo *philo, int duration)
{
	long	start;
	long	is_dead;
	long	timestamp;

	pthread_mutex_lock(philo->mutexes[MUTEX_DEAD]);
	is_dead = pthread_mutex_lock(philo->mutexes[MUTEX_DEAD]);
	pthread_mutex_unlock(philo->mutexes[MUTEX_DEAD]);
	if (!is_dead)
		return ;
	timestamp = current_timestamp_ms() - philo->start_time;
	printf("%li %i %s\n", timestamp, philo->id, action);
	start = current_timestamp_ms();
	while (current_timestamp_ms() - start < duration)
	{
		pthread_mutex_lock(philo->mutexes[MUTEX_DEAD]);
		is_dead = *philo->dead_flag;
		pthread_mutex_unlock(philo->mutexes[MUTEX_DEAD]);
		//if (is_dead || check_dead(philo))
			return ;
		usleep(100);
	}
}*/
/*
void	fast_log(const char *action, t_philoeters *param)
{
}
*/

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

long	current_timestamp_ms(void)
{
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}
