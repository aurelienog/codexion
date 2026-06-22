/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:05:19 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 11:15:31 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_error	join_threads(t_monitor *monitor)
{
	size_t			i;
	t_error			error;
	t_simulation	*simulation;

	i = 0;
	error = ERROR_NONE;
	simulation = monitor->simulation;
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
	size_t		i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(
				&simulation->coders[i].thread,
				NULL,
				&routine,
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

t_error	start_monitor(t_monitor *monitor)
{
	if (pthread_create(&monitor->thread, NULL, &watch, (void *)monitor) != 0)
		return (ERROR_THREAD);
	return (ERROR_NONE);
}

t_error	run_app(t_simulation *simulation)
{
	t_error		error;
	t_monitor	monitor;
	size_t		i;

	monitor.simulation = simulation;
	error = start_coders(simulation);
	if (error != ERROR_NONE)
		return (error);
	error = start_monitor(&monitor);
	if (error != ERROR_NONE)
	{
		pthread_mutex_lock(&simulation->state_mutex);
		simulation->termination_flag = 1;
		pthread_mutex_unlock(&simulation->state_mutex);
		i = simulation->created_coders;
		while (i > 0)
			pthread_join(simulation->coders[--i].thread, NULL);
		return (error);
	}
	return (join_threads(&monitor));
}

int	main(int argc, char **argv)
{
	t_error			error;
	t_simulation	*simulation;

	simulation = build_simulation(argc, argv, &error);
	if (!simulation)
	{
		if (error == ERROR_ARGC)
			print_usage();
		else
			print_error(error);
		return (EXIT_FAILURE);
	}
	error = run_app(simulation);
	if (error != ERROR_NONE)
		print_error(error);
	destroy_simulation(simulation);
	return (error != ERROR_NONE);
}
