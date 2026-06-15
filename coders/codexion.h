/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:39:55 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/15 12:34:22 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define SUCCESS 0
# define ERROR_MUTEX 1
# define ERROR_MALLOC 2
# define ERROR_THREAD 3

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

typedef enum e_scheduler_type	t_schedulerType;
typedef struct s_configuration	t_configuration;
typedef struct s_simulation	t_simulation;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;
typedef struct s_request	t_request;
typedef struct s_monitor	t_monitor;


typedef struct s_simulation
{
	t_coder			*coders;
	t_dongle		*dongles;

	t_configuration	config;

	long long		start_time;

	pthread_mutex_t	simulation_mutex;
	pthread_mutex_t	print_mutex;

	int				termination_flag;
}	t_simulation;

typedef struct s_configuration
{
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	long long		dongle_cooldown;

	int				number_of_compiles_required;
	int				number_of_coders;

	t_schedulerType	scheduler;
}	t_configuration;

typedef struct s_coder
{
	t_dongle		*left;
	t_dongle		*right;

	t_simulation	*simulation;
	pthread_t		thread;

	long long		last_compile_start;

	int				id;
	int				compiles_count;
	int				finished;
}	t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;

	long long		release_time;

	t_request		*waiting_queue;

	int				is_available;
}	t_dongle;

typedef enum e_scheduler_type
{
	FIFO,
	EDF
}	t_schedulerType;

typedef struct s_monitor
{
	t_simulation	*simulation;
	pthread_t		thread;
}	t_monitor;

typedef struct s_request
{
	t_coder		*coder;
	long long	arrival_time;
	long long	deadline;
}	t_request;
#endif