/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:02:17 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/24 13:29:17 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_error	init_dongle(t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (ERROR_MUTEX);
	dongle->is_available = 1;
	dongle->release_time = 0;
	dongle->queue.data = NULL;
	dongle->queue.size = 0;
	dongle->queue.capacity = 0;
	return (ERROR_NONE);
}

t_error	init_dongles(t_simulation *simulation)
{
	int		i;
	t_error	error;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		error = init_dongle(&simulation->dongles[i]);
		if (error != ERROR_NONE)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&simulation->dongles[i].mutex);
			}
			return (error);
		}
		i++;
	}
	return (ERROR_NONE);
}
