/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:20 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/23 14:05:31 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_for_turn(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->scheduler_mutex);
	while (!can_compile(coder) && !simulation_finished(coder->simulation))
		pthread_cond_wait(&coder->simulation->scheduler_cond,
			&coder->simulation->scheduler_mutex);
	pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
}

static t_request	create_request(t_coder *coder)
{
	t_request		request;
	t_simulation	*simulation;

	simulation = coder->simulation;
	request.coder = coder;
	request.arrival_time = get_time_ms();
	pthread_mutex_lock(&coder->simulation->state_mutex);
	request.order = simulation->request_counter++;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	request.deadline = (coder->last_compile_start
			+ simulation->config.time_to_burnout);
	return (request);
}

void	request_compile(t_coder *coder)
{
	t_request	request;

	lock_both_dongles(coder);
	request = create_request(coder);
	enqueue_request(coder->left, &request);
	enqueue_request(coder->right, &request);
	unlock_both_dongles(coder);

	wait_for_turn(coder);
	take_dongles(coder);
	dequeue_request(coder->left);
	dequeue_request(coder->right);
}

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->simulation->print_mutex);
	print_status(coder, "is compiling");
	pthread_mutex_unlock(&coder->simulation->print_mutex);
	pthread_mutex_lock(&coder->simulation->state_mutex);
	usleep(coder->simulation->config.time_to_compile * 1000);
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	pthread_mutex_lock(&coder->mutex);
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->mutex);
}
//compile()
// debug()
// refactor()
