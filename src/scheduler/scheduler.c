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

t_error	enqueue_request_locked(t_dongle *dongle, t_request *request)
{
	return (heap_push(&dongle->queue, request));
}

t_request	dequeue_request_locked(t_dongle *dongle)
{
	return (heap_pop(&dongle->queue));
}

int	is_next(t_dongle *dongle, t_coder *coder)
{
	t_request	*top;

	top = heap_peek(&dongle->queue);
	if (!top)
		return (0);
	return (top->coder == coder);
}

static int	cooldown_expired(t_dongle *dongle, long long cooldown)
{
	return (get_time_ms() >= dongle->release_time + cooldown);
}

int	can_compile_locked(t_coder *coder)
{
	int			result;
	long long	cooldown;

	cooldown = coder->simulation->config.dongle_cooldown;
	result = (
			coder->left->is_available
			&& coder->right->is_available
			&& cooldown_expired(coder->left, cooldown)
			&& cooldown_expired(coder->right, cooldown)
			&& is_next(coder->left, coder)
			&& is_next(coder->right, coder)
			);
	return (result);
}
