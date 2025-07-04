/* ************************************************************************** */
/*										*/
/*							 ::::::::	   */
/*   philosophers.h					  :+:	:+:	   */
/*							  +:+		   */
/*   By: makurek <marvin@42.fr>			+#+			*/
/*							+#+			 */
/*   Created: 2025/04/28 14:06:43 by makurek	#+#	#+#		*/
/*   Updated: 2025/04/28 14:17:58 by makurek	########   odam.nl	*/
/*										*/
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define TIMER_DEATH 0
#define TIMER_EATING 1
#define TIMER_SLEEPING 2
#define MEAL_GOAL 3

#define MUTEX_PRINT 0
#define MUTEX_DEATH 1
#define MUTEX_MEAL  2

typedef struct s_parameters
{
	pthread_mutex_t *forks;
	pthread_mutex_t *mutexes[3]; // [PRINT, DEATH, MEAL]
	int				timers[3];
	int				fork_state[200];
	int				*death_flag;
	long			*last_meal;
	int				philo_id;
	long			start_time;
} t_parameters;

long	current_timestamp_ms(void);
int		check_death(t_parameters *param);
void	log_and_sleep(const char *action, t_parameters *param, int duration);
void	*routine(void *arg);
void    ft_putnbr_fd(long n, int fd);
void    fast_log(const char *action, t_parameters *param);
