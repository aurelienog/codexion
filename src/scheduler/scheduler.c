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
	simulation->scheduler.reserved[coder->left->id - 1] = 1;
	simulation->scheduler.reserved[coder->right->id - 1] = 1;
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

static void	process_request(t_simulation *simulation,
	t_request request, size_t *rejected)
{
	t_coder			*coder;
	int				permission;

	coder = request.coder;
	pthread_mutex_lock(&coder->mutex);
	permission = coder->permission_to_compile;
	pthread_mutex_unlock(&coder->mutex);
	if (permission)
		return ;
	if (can_schedule(coder, simulation->scheduler.reserved))
		grant_permission(simulation, coder);
	else
		simulation->scheduler.pending[(*rejected)++] = request;
}

void	scheduler_run(t_simulation *simulation)
{
	t_request_heap	*heap;
	size_t			rejected_count;
	size_t			i;

	heap = &simulation->scheduler.request_heap;
	memset(simulation->scheduler.reserved, 0,
		sizeof(int) * simulation->config.number_of_coders);
	rejected_count = 0;
	while (heap->size > 0)
		process_request(simulation, heap_pop(heap), &rejected_count);
	i = 0;
	while (i < rejected_count)
	{
		heap_push(heap, &simulation->scheduler.pending[i]);
		i++;
	}
}

void	*scheduler_routine(void *arg)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)arg;
	while (!simulation_finished(simulation))
	{
		pthread_mutex_lock(&simulation->scheduler.mutex);
		scheduler_run(simulation);
		pthread_cond_broadcast(&simulation->scheduler.cond);
		pthread_mutex_unlock(&simulation->scheduler.mutex);
		usleep(1500);
	}
	return (NULL);
}
