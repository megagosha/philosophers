//
// Created by George Tevosov on 13.07.2021.
//

#include "philo.h"

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

/*
 * lock for num_phil + 1 -> die flag
 * lock for num_phil + 2 -> count phils with num2eat 0
 */

int	alloc_mutex(t_params *params)
{
	int	i;

	i = 0;
	params->lock = malloc(sizeof(pthread_mutex_t *) * (params->num_phil + 1));
	if (params->lock == NULL)
		return (1);
	while (i <= params->num_phil)
	{
		params->lock[i] = malloc(sizeof(pthread_mutex_t));
		if (params->lock[i] == NULL)
			return (1);
		if (pthread_mutex_init(params->lock[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

int	init_params(t_params *params, int argc, char **argv)
{
	params->num_phil = ft_atoi(argv[1]);
	params->t2d = ft_atoi(argv[2]);
	params->t2e = ft_atoi(argv[3]);
	params->t2s = ft_atoi(argv[4]);
	params->die = 0;
	if (argc == 6)
		params->nte = ft_atoi(argv[5]);
	else
		params->nte = -1;
	params->r2d = 0;
	params->tid = malloc(sizeof(pthread_t) * params->num_phil);
	params->phil = malloc(sizeof(t_phil) * params->num_phil);
	if (params->tid == NULL || params->phil == NULL)
		return (1);
	return (alloc_mutex(params));
}

void	return_fork(t_phil *ph)
{
	int	l_fork;
	int	r_fork;

	if (ph->num_phil <= 1)
		return ;
	if (ph->num_phil == ph->uni_num)
		r_fork = ph->num_phil - 1;
	else
		r_fork = ph->uni_num - 1;
	if (ph->uni_num == 1)
		l_fork = ph->num_phil - 1;
	else
		l_fork = ph->uni_num - 2;
	pthread_mutex_unlock(ph->lock[l_fork]);
	pthread_mutex_unlock(ph->lock[r_fork]);
	return ;
}

int	get_fork(t_phil *ph)
{
	int	l_fork;
	int	r_fork;

	if (ph->num_phil <= 1)
		return (0);
	if (ph->num_phil == ph->uni_num)
		r_fork = ph->num_phil - 1;
	else
		r_fork = ph->uni_num - 1;
	if (ph->uni_num == 1)
		l_fork = ph->num_phil - 1;
	else
		l_fork = ph->uni_num - 2;
	pthread_mutex_lock(ph->lock[l_fork]);
	pthread_mutex_lock(ph->lock[r_fork]);
	return (1);
}

uint64_t	get_time(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL))
		return (0);
	return (t.tv_sec * (uint64_t) 1000 + t.tv_usec / 1000);
}

int	check2die(t_phil *ph)
{
	pthread_mutex_lock(ph->lock[ph->num_phil]);
	if (*ph->die == 1)
	{
		pthread_mutex_unlock(ph->lock[ph->num_phil]);
		return (1);
	}
	if (*ph->rf == ph->num_phil)
	{
		pthread_mutex_unlock(ph->lock[ph->num_phil]);
		return (1);
	}
	pthread_mutex_unlock(ph->lock[ph->num_phil]);
	return (0);
}

void check2print(t_phil *ph, uint64_t time, char *str)
{
	if (!check2die(ph))
		printf("[%llu] %d %s\n",
			   get_time() - time, ph->uni_num, str);
	return;
}
int	eat(t_phil *ph, uint64_t time)
{
	if (get_fork(ph))
		check2print(ph, time, "has taken a fork");
//		printf("[%llu] %d has taken a fork\n", get_time() - time, ph->uni_num);
	else
		return (-1);
	ph->tlm = get_time();
	if (!ph->tlm)
		return (1);
	check2print(ph, time, "is eating");
//	printf("[%llu] %d is eating\n", get_time() - time, ph->uni_num);
	usleep(ph->t2e * 1000);
	return_fork(ph);
	if (ph->nte == 1)
	{
		pthread_mutex_lock(ph->lock[ph->num_phil]);
		*ph->rf += 1;
		pthread_mutex_unlock(ph->lock[ph->num_phil]);
	}
	if (ph->nte > 0)
		ph->nte--;
	return (0);
}

void	set_death(t_phil *ph, uint64_t c_time)
{
	pthread_mutex_lock(ph->lock[ph->num_phil]);
	*ph->die = 1;
	printf("[%llu] %d died!\n",
		   get_time() - c_time, ph->uni_num);
	pthread_mutex_unlock(ph->lock[ph->num_phil]);
	return ;
}

void 	*return_death(uint64_t c_time, t_phil *ph)
{
	set_death(ph, c_time);
//	if (!check2die(ph))
//		check2print(ph, c_time, "died");
//	printf("[%llu] %d died!\n",
//		   get_time() - c_time, ph->uni_num);
	return (NULL);
}

void	*run_sim(void *phil)
{
	t_phil		*ph;
	uint64_t	c_time;
	int			res;

	c_time = get_time();
	ph = (t_phil *) phil;
	ph->tlm = get_time();
	while (1)
	{
		if (check2die(ph))
			return (NULL);
		if (ph->t2e > ph->t2d || get_time() - ph->tlm > ph->t2d)
			return (return_death(c_time, ph));
		res = eat(ph, c_time);
		if (res == 1)
			return (return_death(c_time, ph));
		else if (res == 0)
		{
			if (get_time() - ph->tlm + ph->t2s > ph->t2d)
				return (return_death(c_time, ph));
			usleep(ph->t2s * 1000);
		}
	}
}

t_phil	create_phil(t_params *params, int num)
{
	t_phil	phil;

	phil.uni_num = num;
	phil.t2d = params->t2d;
	phil.t2e = params->t2e;
	phil.t2s = params->t2s;
	phil.nte = params->nte;
	phil.lock = params->lock;
	phil.die = &params->die;
	phil.num_phil = params->num_phil;
	phil.rf = &params->r2d;
	return (phil);
}

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
	free(p->tid);
	free(p->phil);
}

int	start_loop(t_params *p)
{
	int			i;

	i = 0;
	while (i < p->num_phil)
	{
		p->phil[i] = create_phil(p, i + 1);
		if (pthread_create(&(p->tid[i]), NULL, &run_sim, &p->phil[i]))
			return (1);
		i++;
	}
	i--;
	while (i >= 0)
	{
		pthread_join(p->tid[i], NULL);
		i--;
	}
	clean_up(p);
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;

	if (argc < 5 || argc > 6)
	{
		printf("Wrong number of arguments\n");
		return (EXIT_FAILURE);
	}
	if (init_params(&params, argc, argv))
		return (EXIT_FAILURE);
	if (start_loop(&params))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
