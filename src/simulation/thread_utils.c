/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 11:10:39 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/29 11:10:42 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_error	start_scheduler(t_simulation *simulation)
{
	if (pthread_create(&simulation->scheduler.thread,
			NULL,
			&scheduler_routine,
			simulation) != 0)
		return (ERROR_THREAD);
	return (ERROR_NONE);
}

t_error	join_threads(t_simulation *simulation)
{
	int				i;
	t_error			error;
	t_monitor		*monitor;
	t_scheduler		*scheduler;

	i = 0;
	error = ERROR_NONE;
	monitor = &simulation->monitor;
	scheduler = &simulation->scheduler;
	if (pthread_join(scheduler->thread, NULL))
		error = ERROR_THREAD;
	if (pthread_join(monitor->thread, NULL) != 0)
		error = ERROR_THREAD;
	while (i < simulation->created_coders)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			error = ERROR_THREAD;
		i++;
	}
	return (error);
}

t_error	start_coders(t_simulation *simulation)
{
	int		i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(
				&simulation->coders[i].thread,
				NULL,
				&coder_routine,
				&simulation->coders[i]
			) != 0)
		{
			pthread_mutex_lock(&simulation->state_mutex);
			simulation->termination_flag = 1;
			pthread_mutex_unlock(&simulation->state_mutex);
			while (i > 0)
				pthread_join(simulation->coders[--i].thread, NULL);
			return (ERROR_THREAD);
		}
		simulation->created_coders++;
		i++;
	}
	return (ERROR_NONE);
}

t_error	start_monitor(t_simulation *simulation)
{
	if (pthread_create(&simulation->monitor.thread,
			NULL,
			&monitor_routine,
			(void *)simulation) != 0)
		return (ERROR_THREAD);
	return (ERROR_NONE);
}
