//
// Created by George Tevosov on 13.07.2021.
//

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
/*
 *
 * t2d - time to die with out food
 * t2e - time to eat
 * t2s - time to sleep
 * nte - optional - number of times to eat
 * fa - forks available
 */

/*
 * Philosopher number 1 is next to
 * philosopher number ’number_of_philosophers’.
 * tlm - time from last meal
 * tls - time from last sleep
 * tlt - time from last thinking
 */

typedef struct s_phil {
	int				uni_num;
	int				num_phil;
	uint64_t		tlm;
	uint64_t		t2d;
	uint64_t		t2e;
	uint64_t		t2s;
	int				nte;
	pthread_mutex_t	**lock;
	int				*die;
	int				*rf;

}	t_phil;

typedef struct s_params {
	int				num_phil;
	unsigned int	t2d;
	unsigned int	t2e;
	unsigned int	t2s;
	int				nte;
	int				r2d;
	int				die;
	pthread_mutex_t	**lock;
	pthread_t		*tid;
	t_phil			*phil;
}	t_params;

#endif
