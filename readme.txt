# the philosophers
Philosophers with mutexes and threads.

## instructions:
- 1 or more philosophers sit around a table.

- The philosophers take turns eating, thinking, and sleeping. each being mutually exclusive.

- To eat each philosopher need 2 forks, they have access to the fork on their left and right.

- The number of forks and philosophers is the same. Hence they cannot all eat at once.

- No fork can be used by multiple philsophers.

- Philosophers should avoid death, eating ASAP.

- Once a philosopher dies the simulation stops.

--------------------------------------------------------------------------------------------------

## Global rules:

- Each philosopher has a number ranging from 1 to number_of_philosophers.

- Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher
N + 1.

-Any state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.

- A displayed state message should not overlap with another message.

- A message announcing a philosopher’s death must be displayed within 10 ms of
their actual death.

--------------------------------------------------------------------------------------------------

## Mandatory part:
Program name
philo

Turn in files:
Makefile, *.h, *.c, in directory philo/

Makefile:
NAME, all, clean, fclean, re

Arguments:
number_of_philosophers
time_to_die 
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]

External functs:
memset, printf, malloc, free, write,
usleep, gettimeofday
pthread_create, pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

Libft authorized: No

-------------------------------------------------

The specific rules for the mandatory part are:

• Each philosopher must be represented as a separate thread.

• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, they will have access to just one fork.

• To prevent philosophers from duplicating forks, you should protect each fork’s state
with a mutexe.

-------------------------------------------------

## Ambiguous rules

- It isn't said when start_time should start. it could both be at the start of each philo or at the start of the program.
- We can have different philosophers starting at different points in their cycle.
