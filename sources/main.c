#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void	*routine(pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	pthread_mutex_lock(fork + 1);
	printf("nearest forks used\n");
	pthread_mutex_unlock(fork);
	pthread_mutex_unlock(fork + 1);
	return (NULL);
}

int main()
{
	pthread_mutex_t	fork[201];
	pthread_t		philosopher[200];
	int				philo_number;
	int				i;

	i = 0;
	philo_number = 2;
	while (i < philo_number + 1)
		pthread_mutex_init(&fork[i++], NULL);
	i = -1;
	while (++i < philo_number)
		pthread_create(&philosopher[i], NULL, routine, &fork[i]);
	printf("After\n");
	while (philo_number)
		pthread_join(philosopher[philo_number-- - 1], NULL);
	while (i)
		pthread_mutex_destroy(&fork[i--]);
	return (0);
}
