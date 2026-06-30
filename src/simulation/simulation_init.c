/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 10:39:47 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/29 10:39:49 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_error	init_mutexes(t_simulation *simulation)
{
	if (pthread_cond_init(&simulation->scheduler.cond, NULL) != 0)
		return (ERROR_MUTEX);
	if (pthread_mutex_init(&simulation->state_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		return (ERROR_MUTEX);
	}
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		pthread_mutex_destroy(&simulation->state_mutex);
		return (ERROR_MUTEX);
	}
	if (pthread_mutex_init(&simulation->scheduler.mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		pthread_mutex_destroy(&simulation->print_mutex);
		pthread_mutex_destroy(&simulation->state_mutex);
		return (ERROR_MUTEX);
	}
	simulation->created_coders = 0;
	simulation->scheduler.request_counter = 0;
	simulation->termination_flag = 0;
	simulation->start_time = get_time_ms();
	return (ERROR_NONE);
}

static void	destroy_mutexes(t_simulation *simulation)
{
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->state_mutex);
	pthread_mutex_destroy(&simulation->scheduler.mutex);
	pthread_cond_destroy(&simulation->scheduler.cond);
}

static void	destroy_dongles_mutexes(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
}

t_error	init_simulation(t_simulation *simulation)
{
	t_error	error;

	error = init_mutexes(simulation);
	if (error != ERROR_NONE)
		return (error);
	error = init_dongles(simulation);
	if (error != ERROR_NONE)
	{
		destroy_mutexes(simulation);
		return (error);
	}
	error = init_coders(simulation);
	if (error != ERROR_NONE)
	{
		destroy_dongles_mutexes(simulation);
		destroy_mutexes(simulation);
		return (error);
	}
	simulation->scheduler.request_heap.data = NULL;
	simulation->scheduler.request_heap.size = 0;
	simulation->scheduler.request_heap.capacity = 0;
	return (ERROR_NONE);
}
