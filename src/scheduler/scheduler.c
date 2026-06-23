/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:59:31 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/23 14:09:42 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_error	*enqueue_request(t_dongle *dongle, t_request *request)
{
	return (heap_push(&dongle->queue, request));
}

t_request	dequeue_request(t_dongle *dongle)
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

int	can_compile(t_coder *coder)
{
	int	result;

	lock_both_dongles(coder);
	result = (
			coder->left->is_available
			&& coder->right->is_available
			&& is_next(coder->left, coder)
			&& is_next(coder->right, coder)
			);
	unlock_both_dongles(coder);
	return (result);
}
