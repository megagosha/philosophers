#include "philo.h"

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
	params->t2d = ft_atoi(argv[2]) * 1000;
	params->t2e = ft_atoi(argv[3]) * 1000;
	params->t2s = ft_atoi(argv[4]) * 1000;
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

uint64_t	get_time(void)
{
	struct timeval	t;

	if (gettimeofday(&t, NULL))
		return (0);
	return (t.tv_sec * (uint64_t)1000000 + t.tv_usec);
}