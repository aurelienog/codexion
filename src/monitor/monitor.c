/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:39:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:28:40 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	check_burnout(t_simulation *simulation)
{
	int			i;
	int			finished;
	long long	elapsed;
	long long	last_compile;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].mutex);
		finished = simulation->coders[i].finished;
		last_compile = simulation->coders[i].last_compile_start;
		pthread_mutex_unlock(&simulation->coders[i].mutex);
		if (finished)
		{
			i++;
			continue ;
		}
		elapsed = get_time_ms() - last_compile;
		if (elapsed >= simulation->config.time_to_burnout)
		{
			set_simulation_finished(simulation);
			print_status(&simulation->coders[i], STATUS_BURNED);
			return ;
		}
		i++;
	}
	return ;
}

static void	check_completion(t_simulation *simulation)
{
	int		i;
	int		finished;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].mutex);
		finished = simulation->coders[i].finished;
		pthread_mutex_unlock(&simulation->coders[i].mutex);
		if (!finished)
			return ;
		i++;
	}
	set_simulation_finished(simulation);
}

void	*watch(void *arg)
{
	t_monitor		*monitor;
	t_simulation	*simulation;

	monitor = (t_monitor *)arg;
	simulation = monitor->simulation;
	while (!simulation_finished(simulation))
	{
		check_burnout(simulation);
		check_completion(simulation);
		usleep(900);
	}
	return (NULL);
}
