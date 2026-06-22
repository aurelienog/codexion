/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:20 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 15:55:32 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	create_request(t_coder *coder)
{
	t_request		request;
	t_simulation	*simulation;

	simulation = coder->simulation;
	request.coder = coder;
	request.arrival_time = get_time_ms();
	pthread_mutex_lock(&simulation->state_mutex);
	request.order = simulation->request_counter++;
	pthread_mutex_unlock(&simulation->state_mutex);
	request.deadline = (coder->last_compile_start
			+ simulation->config.time_to_burnout);
	return (request);
}

void	request_compile(t_coder *coder)
{
	t_request	request;

	request = create_request(coder);
	enqueue_request(coder->left, &request);
	enqueue_request(coder->right, &request);
	while (!can_compile(coder) && !simulation_finished())
		pthread_cond_wait(&coder->simulation->scheduler.cond,
				&coder->simulation->scheduler.mutex);
scheduler_mutex
	//take_dongles;
	dequeue_request(coder->left, &request);
	dequeue_request(coder->right, &request);
}

void	release_dongles(t_coder *coder)
{
	coder->left->is_available = 1;
	coder->right->is_available = 1;
	pthread_mutex_lock(&coder->simulation->scheduler.mutex);
	pthread_cond_broadcast(&coder->simulation->scheduler.cond);
	pthread_mutex_unlock(&coder->simulation->scheduler.mutex);
}
//compile()

// debug()
// refactor()
