#include "philo.h"

void	clean_up(t_params *p)
{
	int	i;

	i = 0;
	while (i <= p->num_phil)
	{
		free(p->lock[i]);
		pthread_mutex_destroy(p->lock[i]);
		i++;
	}
	free(p->lock);
	free(p->forks);
	free(p->tid);
	free(p->phil);
}

void 	*return_death(uint64_t c_time, t_phil *ph)
{
	set_death(ph, c_time);
	return (NULL);
}

void	ft_sleep(t_phil *ph, uint64_t c_time)
{
	check2print("is is sleeping", ph, c_time);
	ft_usleep(ph->t2s);
	return ;
}

void	ft_usleep(uint64_t t)
{
	uint64_t	start;
	uint64_t	end;

	start = get_time();
	end = start + t;
	while (1)
	{
		if (get_time() >= end)
			return ;
		usleep(50);
	}
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	res;

	res = 0;
	sign = 1;
	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
		   || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] > 47 && str[i] < 58)
	{
		res = (res * 10) + (str[i] - 48);
		i++;
	}
	return (res * sign);
}
