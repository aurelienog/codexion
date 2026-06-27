/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 11:39:55 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:22:56 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>

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
typedef struct s_request_heap	t_request_heap;
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

typedef enum e_status
{
	STATUS_COMPILING,
	STATUS_DEBUGGING,
	STATUS_REFACTORING,
	STATUS_TAKE_DONGLE,
	STATUS_BURNED,
	STATUS_FINISHED
}	t_status;
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

typedef struct s_scheduler
{
	t_simulation	*simulation;
	pthread_t			thread;
}	t_scheduler;
/**
@brief Pending request for a dongle.
*/
typedef struct s_request
{
	t_coder		*coder;
	long long	arrival_time;
	long long	deadline;
	size_t		order;
}	t_request;

typedef struct s_request_heap
{
	t_request	*data;
	size_t		size;
	size_t		capacity;
}	t_request_heap;

/**
@brief Global simulation state.
*/
typedef struct s_simulation
{
	t_coder			*coders;
	t_dongle		*dongles;
	t_configuration	config;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	scheduler_mutex;
	pthread_cond_t	scheduler_cond;
	t_request_heap	request_heap;
	t_request		*pending;
	long long		start_time;
	int				*reserved;
	int				termination_flag;
	int				created_coders;
	size_t			request_counter;
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
	pthread_mutex_t	mutex;
	long long		last_compile_start;
	int				id;
	int				waiting_for_compile;
	int				permission_to_compile;
	int				compiles_count;
	int				finished;
}	t_coder;

/**
@brief Shared resource required by coders.
*/
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long long		release_time;
	int				id;
	int				is_available;
	int				cooldown_expired_notified;
}	t_dongle;
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

void			set_simulation_finished(t_simulation *simulation);
int				simulation_finished(t_simulation *simulation);
t_error	run_app(t_simulation *simulation);
void		*scheduler_routine(void *arg);

/* ************************************************************************** */
/*                                  Monitor                                   */
/* ************************************************************************** */

void			*watch(void *arg);

/* ************************************************************************** */
/*                                   Coder                                    */
/* ************************************************************************** */

t_error			init_coders(t_simulation *simulation);

void			*routine(void *arg);
void			request_compile(t_coder *coder);
void			compile(t_coder *coder);
void			debug(t_coder *coder);
void			refactor(t_coder *coder);

/* ************************************************************************** */
/*                                  Dongle                                    */
/* ************************************************************************** */

t_error			init_dongles(t_simulation *simulation);
void			lock_both_dongles(t_coder *coder);
void			unlock_both_dongles(t_coder *coder);
void			take_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);

/* ************************************************************************** */
/*                                 Scheduler                                  */
/* ************************************************************************** */

int				request_has_priority(t_request *a, t_request *b);
t_error		scheduler_enqueue(t_coder *coder);
void			scheduler_run(t_simulation *simulation);

/* ************************************************************************** */
/*                                  Heap                                      */
/* ************************************************************************** */

t_request		*heap_peek(t_request_heap *heap);
t_request		heap_pop(t_request_heap *heap);
t_error			heap_push(t_request_heap *heap, t_request *request);
t_error			heap_grow(t_request_heap *heap);
void			swap(t_request *a, t_request *b);

/* ************************************************************************** */
/*                                  IO                                     */
/* ************************************************************************** */

void			print_error(t_error error);
void			print_usage(void);
void			print_status(t_coder *coder, t_status status);
t_error			parse_input(int argc, char **argv);
long long		get_time_ms(void);
#endif
