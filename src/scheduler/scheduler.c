/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:59:31 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:29:44 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	grant_permission(t_simulation *simulation, t_coder *coder)
{
	simulation->reserved[coder->left->id - 1] = 1;
	simulation->reserved[coder->right->id - 1] = 1;
	take_dongles(coder);
	pthread_mutex_lock(&coder->mutex);
	coder->waiting_for_compile = 0;
	coder->permission_to_compile = 1;
	pthread_mutex_unlock(&coder->mutex);
}

static int	can_schedule(t_coder *coder, int *reserved)
{
	long long	cooldown;
	int			left;
	int			is_left_ready;
	int			right;
	int			is_right_ready;

	cooldown = coder->simulation->config.dongle_cooldown;
	left = coder->left->id - 1;
	right = coder->right->id - 1;
	lock_both_dongles(coder);
	is_left_ready = get_time_ms() >= coder->left->release_time + cooldown;
	is_right_ready = get_time_ms() >= coder->right->release_time + cooldown;
	if (!coder->left->is_available || !coder->right->is_available
	|| reserved[left] || reserved[right]
	|| !is_left_ready || !is_right_ready)
	{
		unlock_both_dongles(coder);
		return (0);
	}
	unlock_both_dongles(coder);
	return (1);
}

static void	process_request(t_simulation *simulation, t_request request, size_t *rejected)
{
	t_coder	*coder;
	int				permission;

	coder = request.coder;
	pthread_mutex_lock(&coder->mutex);
	permission = coder->permission_to_compile;
	pthread_mutex_unlock(&coder->mutex);
	if (permission)
		return ;
	if (can_schedule(coder, simulation->reserved))
		grant_permission(simulation, coder);
	else
		simulation->pending[(*rejected)++] = request;
}

t_error	scheduler_enqueue(t_coder *coder)
{
	t_request	request;
	t_simulation		*simulation;

	simulation = coder->simulation;
	request.coder = coder;
	request.arrival_time = get_time_ms();
	pthread_mutex_lock(&coder->mutex);
	if (coder->waiting_for_compile)
	{
		pthread_mutex_unlock(&coder->mutex);
		return (ERROR_NONE);
	}
	coder->waiting_for_compile = 1;
	request.deadline = coder->last_compile_start + simulation->config.time_to_burnout;
	pthread_mutex_unlock(&coder->mutex);
	request.order = simulation->request_counter++;
	return (heap_push(&simulation->request_heap, &request));
}

void	scheduler_run(t_simulation *simulation)
{
	t_request_heap	*heap;
	size_t	rejected_count;
	size_t	i;

	heap = &simulation->request_heap;
	memset(simulation->reserved, 0,
		sizeof(int) * simulation->config.number_of_coders);
	rejected_count = 0;
	while (heap->size > 0)
		process_request(simulation, heap_pop(heap), &rejected_count);
	i = 0;
	while (i < rejected_count)
	{
		heap_push(heap, &simulation->pending[i]);
		i++;
	}
	pthread_cond_broadcast(&simulation->scheduler_cond);
}

void	*scheduler_routine(void *arg)
{
	t_scheduler		*scheduler;
	t_simulation	*simulation;

	scheduler = arg;
	simulation = scheduler->simulation;
	while (!simulation_finished(simulation))
	{
		pthread_mutex_lock(&simulation->scheduler_mutex);
		scheduler_run(simulation);
		pthread_mutex_unlock(&simulation->scheduler_mutex);
		usleep(1500);
	}
	return (NULL);
}