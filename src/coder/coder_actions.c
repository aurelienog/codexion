/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:20 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/24 13:34:25 by aunoguei         ###   ########.fr       */
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

void	request_compile(t_coder *coder)
{
	t_request		request;
	t_simulation	*simulation;

	simulation = coder->simulation;
	request = create_request(coder);
	pthread_mutex_lock(&simulation->scheduler_mutex);
	lock_both_dongles(coder);
	enqueue_request_locked(coder->left, &request);
	enqueue_request_locked(coder->right, &request);
	unlock_both_dongles(coder);
	while (!simulation_finished(coder->simulation))
	{
		lock_both_dongles(coder);
		if (simulation_finished(simulation))
		{
			unlock_both_dongles(coder);
			pthread_mutex_unlock(&simulation->scheduler_mutex);
			return ;
		}
		if (can_compile_locked(coder))
		{
			take_dongles_locked(coder);
			dequeue_request_locked(coder->left);
			dequeue_request_locked(coder->right);
			unlock_both_dongles(coder);
			pthread_mutex_unlock(&simulation->scheduler_mutex);
			return ;
		}
		unlock_both_dongles(coder);
		pthread_cond_wait(&coder->simulation->scheduler_cond,
			&coder->simulation->scheduler_mutex);
	}
	pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
}

void	compile(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	print_status(coder, STATUS_COMPILING);
	usleep(coder->simulation->config.time_to_compile * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->compiles_count++;
	if (coder->compiles_count >= coder->simulation->config.number_of_compiles_required)
		coder->finished = 1;
	pthread_mutex_unlock(&coder->mutex);
}
void debug(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	print_status(coder, STATUS_DEBUGGING);
	usleep(coder->simulation->config.time_to_debug * 1000);
}
void refactor(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	print_status(coder, STATUS_REFACTORING);
	usleep(coder->simulation->config.time_to_refactor * 1000);
}
