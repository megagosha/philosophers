#include "philo.h"

int	eat(t_phil *ph, uint64_t time)
{
	if (get_fork(ph))
		printf("[%llu] %d has taken a fork\n",
			   (get_time() - time) / 1000, ph->uni_num);
	else
		return (-1);
	if (get_time() - ph->tlm > ph->t2d)
		return (-1);
	ph->tlm = get_time();
	if (!ph->tlm)
		return (1);
	printf("[%llu] %d is eating\n",
		   (get_time() - time) / 1000, ph->uni_num);
	ft_usleep(ph->t2e);
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
