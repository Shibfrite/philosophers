#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_parameters
{
    pthread_mutex_t *forks;
    int             timers[3];        // 0: eat, 1: sleep, 2: think
    int             forks_id[2];      // left and right fork indices
    int             philosopher_id;
    long            start_time;
    pthread_mutex_t *print_mutex;
    pthread_mutex_t *death_mutex;
    pthread_mutex_t *meal_mutex;
    int             *death_flag;
    long            *last_meal;
    int             time_to_die;
} t_parameters;

long current_timestamp_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void log_action(const char *action, t_parameters *param)
{
    long timestamp = current_timestamp_ms() - param->start_time;
    pthread_mutex_lock(param->print_mutex);
    pthread_mutex_lock(param->death_mutex);
    if (!(*param->death_flag))
        printf("%ld %d %s\n", timestamp, param->philosopher_id + 1, action);
    pthread_mutex_unlock(param->death_mutex);
    pthread_mutex_unlock(param->print_mutex);
    fflush(stdout);
}

int check_death(t_parameters *param)
{
    long now = current_timestamp_ms();
    pthread_mutex_lock(param->death_mutex);
    pthread_mutex_lock(param->meal_mutex);
    if (!(*param->death_flag) && (now - *param->last_meal > param->time_to_die))
    {
        *param->death_flag = 1;
        pthread_mutex_unlock(param->meal_mutex);
        pthread_mutex_lock(param->print_mutex);
        printf("%ld %d died\n", now - param->start_time, param->philosopher_id + 1);
        fflush(stdout);
        pthread_mutex_unlock(param->print_mutex);
        pthread_mutex_unlock(param->death_mutex);
        return 1;
    }
    pthread_mutex_unlock(param->meal_mutex);
    pthread_mutex_unlock(param->death_mutex);
    return 0;
}

void precise_usleep(int duration, t_parameters *param)
{
    long start = current_timestamp_ms();
    while (current_timestamp_ms() - start < duration)
    {
        pthread_mutex_lock(param->death_mutex);
        int local_death = *param->death_flag;
        pthread_mutex_unlock(param->death_mutex);
        if (local_death || check_death(param))
            return;
        usleep(100);
    }
}

void *routine(void *arg)
{
    t_parameters *param = (t_parameters *)arg;
    pthread_mutex_t *first_fork, *second_fork;

    // Prevent deadlock: odd philosophers pick right fork first
    if (param->philosopher_id % 2 == 0)
    {
        first_fork = &param->forks[param->forks_id[0]];
        second_fork = &param->forks[param->forks_id[1]];
    }
    else
    {
        first_fork = &param->forks[param->forks_id[1]];
        second_fork = &param->forks[param->forks_id[0]];
    }

    pthread_mutex_lock(param->meal_mutex);
    *param->last_meal = current_timestamp_ms();
    pthread_mutex_unlock(param->meal_mutex);

    while (1)
    {
        pthread_mutex_lock(param->death_mutex);
        int local_death = *param->death_flag;
        pthread_mutex_unlock(param->death_mutex);
        if (local_death || check_death(param))
            break;

        log_action("is thinking", param);
        precise_usleep(param->timtricaduceeers[2], param);

        pthread_mutex_lock(first_fork);
        pthread_mutex_lock(second_fork);

        log_action("is eating", param);
        pthread_mutex_lock(param->meal_mutex);
        *param->last_meal = current_timestamp_ms();
        pthread_mutex_unlock(param->meal_mutex);
        precise_usleep(param->timers[0], param);

        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);

        log_action("is sleeping", param);
        precise_usleep(param->timers[1], param);
    }
    return NULL;
}

int main()
{
    pthread_mutex_t fork[200];
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    pthread_mutex_t meal_mutex[200];
    pthread_t       philosopher[200];
    t_parameters    param[200];
    int             death_flag = 0;
    long            last_meal[200];
    int             philo_number = 5;
    int             i = 0;
    long            start_time = current_timestamp_ms();

    pthread_mutex_init(&print_mutex, NULL);
    pthread_mutex_init(&death_mutex, NULL);
    while (i < philo_number)
    {
        pthread_mutex_init(&fork[i], NULL);
        pthread_mutex_init(&meal_mutex[i], NULL);
        i++;
    }

    i = 0;
    while (i < philo_number)
    {
        param[i].forks = fork;
        param[i].philosopher_id = i;
        param[i].forks_id[0] = i;
        param[i].forks_id[1] = (i + 1) % philo_number;
        param[i].timers[0] = 200;     // eat (ms)
        param[i].timers[1] = 100;     // sleep (ms)
        param[i].timers[2] = 100;     // think (ms)
        param[i].time_to_die = 80;   // die (ms)
        param[i].start_time = start_time;
        param[i].print_mutex = &print_mutex;
        param[i].death_mutex = &death_mutex;
        param[i].meal_mutex = &meal_mutex[i];
        param[i].death_flag = &death_flag;
        param[i].last_meal = &last_meal[i];
        last_meal[i] = start_time;
        pthread_create(&philosopher[i], NULL, routine, &param[i]);
        i++;
    }

    for (i = 0; i < philo_number; i++)
        pthread_join(philosopher[i], NULL);
    for (i = 0; i < philo_number; i++)
    {
        pthread_mutex_destroy(&fork[i]);
        pthread_mutex_destroy(&meal_mutex[i]);
    }
    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&death_mutex);

    return 0;
}
