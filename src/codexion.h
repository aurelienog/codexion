/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:39:55 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 14:50:53 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>

/* ************************************************************************** */
/*                           Forward Declarations                             */
/* ************************************************************************** */

typedef enum e_scheduler_type	t_scheduler_type;
typedef enum e_error			t_error;
typedef struct s_configuration	t_configuration;
typedef struct s_simulation		t_simulation;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;
typedef struct s_request		t_request;
typedef struct s_monitor		t_monitor;

/* ************************************************************************** */
/*                                  Enums                                     */
/* ************************************************************************** */

typedef enum e_error
{
	ERROR_NONE,
	ERROR_ARGC,
	ERROR_VALUE,
	ERROR_SCHEDULER,
	ERROR_MALLOC,
	ERROR_MUTEX,
	ERROR_THREAD,
}	t_error;

/**
@brief Scheduling policy used to assign dongles.
*/
typedef enum e_scheduler_type
{
	FIFO,
	EDF
}	t_scheduler_type;

/* ************************************************************************** */
/*                                  Structures                                */
/* ************************************************************************** */

/**
@brief Global simulation configuration.
*/
typedef struct s_configuration
{
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	long long			dongle_cooldown;

	int					number_of_compiles_required;
	int					number_of_coders;

	t_scheduler_type	scheduler;
}	t_configuration;

/**
@brief Global simulation state.
*/
typedef struct s_simulation
{
	t_coder			*coders;
	t_dongle		*dongles;

	t_configuration	config;

	long long		start_time;

	pthread_mutex_t	simulation_mutex;
	pthread_mutex_t	print_mutex;

	int				termination_flag;
	int				created_coders;
}	t_simulation;

/**
@brief Represents a coder participating in the simulation.
*/
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

/**
@brief Shared resource required by coders.
*/
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t		cond;
	t_request_heap		queue;
	long long		release_time;
	int				is_available;
}	t_dongle;

/**
@brief Pending request for a dongle.
*/
typedef struct s_request
{
	t_coder		*coder;
	long long	arrival_time;
	long long	deadline;
	size_t	order;
}	t_request;

typedef struct s_request_heap
{
	t_request	*data;
	size_t		size;
	size_t		capacity;
}	t_request_heap;

/**
@brief Dedicated monitoring thread.
*/
typedef struct s_monitor
{
	t_simulation	*simulation;
	pthread_t		thread;
}	t_monitor;

/* ************************************************************************** */
/*                              Configuration                                 */
/* ************************************************************************** */

t_configuration	create_config(char **argv);

/* ************************************************************************** */
/*                                Simulation                                  */
/* ************************************************************************** */

t_simulation	*build_simulation(int argc, char **argv, t_error *error);
void			destroy_simulation(t_simulation *simulation);

/* ************************************************************************** */
/*                                   Coder                                    */
/* ************************************************************************** */

void			init_coder(t_dongle *left_dongle,
					t_dongle *right_dongle,
					t_simulation *simulation,
					size_t i);

void			init_coders(t_simulation *simulation);

/* ************************************************************************** */
/*                                  Dongle                                    */
/* ************************************************************************** */
int				init_dongle(t_dongle *dongle, t_simulation *simulation);
int				init_dongles(t_simulation *simulation);

#endif
