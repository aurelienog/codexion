/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:52:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:47:48 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_simulation(t_simulation *simulation)
{
	free(simulation->scheduler.pending);
	free(simulation->scheduler.reserved);
	free(simulation->dongles);
	free(simulation->coders);
	free(simulation);
}

static int	init_scheduler(t_simulation *simulation, size_t coders_number)
{
	simulation->scheduler.pending = malloc(sizeof(t_request) * coders_number);
	if (!simulation->scheduler.pending)
		return (0);
	simulation->scheduler.reserved = malloc(sizeof(int) * coders_number);
	if (!simulation->scheduler.reserved)
		return (free(simulation->scheduler.pending), 0);
	return (1);
}

static t_simulation	*create_simulation(t_configuration config)
{
	t_simulation	*simulation;
	size_t			coders_number;

	simulation = calloc(1, sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	coders_number = config.number_of_coders;
	simulation->coders = malloc(sizeof(t_coder) * coders_number);
	simulation->dongles = malloc(sizeof(t_dongle) * coders_number);
	if (!simulation->coders || !simulation->dongles)
		return (free_simulation(simulation), NULL);
	if (!init_scheduler(simulation, coders_number))
		return (free_simulation(simulation), NULL);
	simulation->config = config;
	return (simulation);
}

t_simulation	*build_simulation(int argc, char **argv, t_error *error)
{
	t_configuration	config;
	t_simulation	*simulation;

	*error = parse_input(argc, argv);
	if (*error != ERROR_NONE)
		return (NULL);
	config = create_config(argv);
	simulation = create_simulation(config);
	if (!simulation)
	{
		*error = ERROR_MALLOC;
		return (NULL);
	}
	*error = init_simulation(simulation);
	if (*error != ERROR_NONE)
	{
		free(simulation->dongles);
		free(simulation->coders);
		free(simulation->scheduler.pending);
		free(simulation->scheduler.reserved);
		free(simulation);
		return (NULL);
	}
	return (simulation);
}
