#include "philosophers.h"

static int	parsing(const char **args, int *args_new);
static int	ft_atoi(const char *str);

int	main(int ac, const char **args)
{
	int	args_num[6];

	if (ac != 5 && ac != 6)
		return (write(2, "arg error\n", 10), 1);
	if (parsing(args, args_num))
		return (1);
	if (args_num[0] == 0)
		return (0);
	if (manage_threads_and_mutexes((const int *)args_num))
		return (1);
	return (0);
}

static int	parsing(const char **args, int *args_num)
{
	int	*start;

	if (!args || !args_num)
		return (1);
	start = args_num;
	while (*++args)
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
	if (args_num - start == 5 && *(args_num - 1) == 0)
		return (1);
	*args_num = 0;
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
