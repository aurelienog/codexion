/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_runner.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 10:35:34 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/29 10:35:36 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	stop_simulation(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->state_mutex);
	simulation->termination_flag = 1;
	pthread_mutex_unlock(&simulation->state_mutex);
}

static void	join_coders(t_simulation *simulation)
{
	size_t	i;

	i = simulation->created_coders;
	while (i > 0)
		pthread_join(simulation->coders[--i].thread, NULL);
}

t_error	run_app(t_simulation *simulation)
{
	t_error	error;

	error = start_scheduler(simulation);
	if (error != ERROR_NONE)
		return (error);
	error = start_coders(simulation);
	if (error != ERROR_NONE)
	{
		stop_simulation(simulation);
		pthread_join(simulation->scheduler.thread, NULL);
		return (error);
	}
	error = start_monitor(simulation);
	if (error != ERROR_NONE)
	{
		stop_simulation(simulation);
		join_coders(simulation);
		return (error);
	}
	return (join_threads(simulation));
}
