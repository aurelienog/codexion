/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile_request.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 11:59:12 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:00:13 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_request	create_request(t_coder *coder)
{
	t_request		request;
	t_simulation	*simulation;
	long long		last_compile;

	simulation = coder->simulation;
	request.coder = coder;
	request.arrival_time = get_time_ms();
	pthread_mutex_lock(&coder->simulation->state_mutex);
	request.order = simulation->request_counter++;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	pthread_mutex_lock(&coder->mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->mutex);
	request.deadline = (last_compile + simulation->config.time_to_burnout);
	return (request);
}

static void	enqueue_compile_request(t_coder *coder, t_request *request)
{
	lock_both_dongles(coder);
	enqueue_request_locked(coder->left, request);
	enqueue_request_locked(coder->right, request);
	unlock_both_dongles(coder);
}

static void	dequeue_compile_request(t_coder *coder)
{
	take_dongles_locked(coder);
	dequeue_request_locked(coder->left);
	dequeue_request_locked(coder->right);
	unlock_both_dongles(coder);
}

void	request_compile(t_coder *coder)
{
	t_request		request;

	request = create_request(coder);
	pthread_mutex_lock(&coder->simulation->scheduler_mutex);
	enqueue_compile_request(coder, &request);
	while (!simulation_finished(coder->simulation))
	{
		lock_both_dongles(coder);
		if (simulation_finished(coder->simulation))
		{
			unlock_both_dongles(coder);
			pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
			return ;
		}
		if (can_compile_locked(coder))
		{
			dequeue_compile_request(coder);
			pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
			return ;
		}
		unlock_both_dongles(coder);
		pthread_cond_wait(&coder->simulation->scheduler_cond,
			&coder->simulation->scheduler_mutex);
	}
	pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
}
