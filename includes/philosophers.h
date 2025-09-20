/* ************************************************************************** */
/*										*/
/*							 ::::::::	   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*							  +:+		   */
/*   By: makurek <marvin@42.fr>			+#+			*/
/*							+#+			 */
/*   Created: 2025/04/28 14:06:43 by makurek	#+#	#+#		*/
/*   Updated: 2025/09/19 17:10:19 by makurek          ###   ####lausanne.ch   */
/*										*/
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

//for args
#define PHILO_NUMBER 0
#define TIMER_DEAD 1
#define TIMER_EATING 2
#define TIMER_SLEEPING 3
#define MEAL_GOAL 4

#define MUTEX_DEAD 0
#define MUTEX_MEAL 1

#define MAX_PHILO 200

typedef struct s_thread_info
{
	pthread_mutex_t	mutex;
	int				state;
}	t_thread_info;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				state;
}	t_fork;

typedef struct s_philo
{
	pthread_mutex_t	mutexes[2];
	pthread_mutex_t	*dead_mutex;
	t_fork			*forks;
	const int		*args;
	int				*dead;
	int				id;
	int				eat_goal;
	long			start_time;
	long			last_meal;

}	t_philo;

typedef struct s_context
{
	const int		*args;
	pthread_mutex_t	dead_mutex;
	pthread_t		thread_id[MAX_PHILO];
	t_philo			philo[MAX_PHILO];
	t_fork			forks[MAX_PHILO];
	int				dead;
}	t_context;

int		manage_threads_and_mutexes(const int *args);
void	*routine(void *arg);
void	release_forks(t_philo *philo, int first, int second);
int		attempt_fork_acquisition(t_philo *philo, int first, int second);
int		check_dead(t_philo *param);
int		log_and_sleep(const char *action, t_philo *philo, int duration);
long	current_timestamp_ms(void);
