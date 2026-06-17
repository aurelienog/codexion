/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:02:17 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 15:53:30 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_error	init_dongle(t_dongle *dongle, t_simulation *simulation)
{
	dongle->is_available = 1;
	dongle->release_time = simulation->start_time;
	dongle->queue = NULL;
	dongle->queue.size = 0;
	dongle->queue.capacity = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (ERROR_MUTEX);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (ERROR_MUTEX);
	}
	return (ERROR_NONE);
}

t_error	init_dongles(t_simulation *simulation)
{
	size_t	i;
	t_error	error;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		error = init_dongle(&simulation->dongles[i], simulation);
		if (error != ERROR_NONE)
		{
			while (i > 0)
			{
				i--;
				pthread_cond_destroy(&simulation->dongles[i].cond);
				pthread_mutex_destroy(&simulation->dongles[i].mutex);
			}
			return (error);
		}
		i++;
	}
	return (ERROR_NONE);
}
