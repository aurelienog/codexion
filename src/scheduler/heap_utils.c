/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_heap_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:48:27 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 10:59:21 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_has_priority(t_request *a, t_request *b)
{
	t_scheduler_type	scheduler;

	scheduler = a->coder->simulation->config.scheduler;
	if (scheduler == EDF)
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
		else
			return (a->coder->id < b->coder->id);
	}
	return (a->order < b->order);
}

t_error	scheduler_enqueue(t_coder *coder)
{
	t_request		request;
	t_simulation	*simulation;

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
	request.deadline = (coder->last_compile_start
			+ simulation->config.time_to_burnout);
	pthread_mutex_unlock(&coder->mutex);
	request.order = simulation->scheduler.request_counter++;
	return (heap_push(&simulation->scheduler.request_heap, &request));
}

void	swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

t_error	heap_grow(t_request_heap *heap)
{
	t_request	*new_data;
	size_t		new_capacity;
	size_t		i;

	if (heap->capacity == 0)
		new_capacity = 8;
	else
		new_capacity = heap->capacity * 2;
	new_data = malloc(sizeof(t_request) * new_capacity);
	if (!new_data)
		return (ERROR_MALLOC);
	i = 0;
	while (i < heap->size)
	{
		new_data[i] = heap->data[i];
		i++;
	}
	free(heap->data);
	heap->data = new_data;
	heap->capacity = new_capacity;
	return (ERROR_NONE);
}
