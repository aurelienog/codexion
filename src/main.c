/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:05:19 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 15:50:30 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_error	run_app(t_simulation *sim)
{
	t_error	error;

	error = start_coders(sim);
	if (error != ERROR_NONE)
		return (error);
	error = start_monitor(sim);
	if (error != ERROR_NONE)
		return (error);
	join_threads(sim);
	return (ERROR_NONE);
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
