/* ************************************************************************** */
/*										*/
/*							 ::::::::	   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*							  +:+		   */
/*   By: makurek <marvin@42.fr>			+#+			*/
/*							+#+			 */
/*   Created: 2025/04/28 14:06:43 by makurek	#+#	#+#		*/
/*   Updated: 2025/09/17 11:33:26 by makurek          ###   ####lausanne.ch   */
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

typedef struct s_parameters
{
	pthread_mutex_t *forks;
	pthread_mutex_t *mutexes[3]; // [PRINT, DEAD, MEAL]
	int				timers[3];
	int				fork_state[200];
	int				*dead_flag;
	long			*last_meal;
	int				philo_id;
	long			start_time;
} t_parameters;

typedef struct s_thread_info
{
	pthread_mutex_t mutex;
	int				state;
} t_thread_info;

typedef struct s_fork
{
	pthread_mutex_t mutex;
	int				state;
} t_fork;

typedef struct s_philo
{
	pthread_mutex_t mutexes[2]; // [DEAD, MEAL] (last meal)
	t_fork			*forks;
	const int		*args;
	int				*dead;
	int				id;
	long			start_time;
	long			last_meal;

} t_philo;

long	current_timestamp_ms(void);
int		check_dead(t_philo *param);
int		log_and_sleep(const char *action, t_philo *philo, int duration);
void	*routine(void *arg);
void    ft_putnbr_fd(long n, int fd);
//void    fast_log(const char *action, t_parameters *param);
