/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:52:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 15:52:10 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_simulation(t_simulation *simulation)
{
// Destruir mutex globales.
// Destruir mutex de cada dongle.
// Liberar dongles.
// Liberar coders.
// Liberar simulation.
}
// Normalmente validarías:
// cantidad exacta de argumentos;
// que los numéricos sean enteros positivos;
// que scheduler sea exactamente:
// "fifo"
// "edf"
// y rechazarías cualquier otra cosa

static t_error	init_simulation(t_simulation *simulation)
{
	t_error	error;

	if (pthread_mutex_init(&simulation->simulation_mutex, NULL) != 0)
		return (ERROR_MUTEX);
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->simulation_mutex);
		return (ERROR_MUTEX);
	}
	simulation->termination_flag = 0;
	simulation->start_time = current_time_ms();
	error = init_dongles(simulation);
	if (error != ERROR_NONE)
	{
		pthread_mutex_destroy(&simulation->print_mutex);
		pthread_mutex_destroy(&simulation->simulation_mutex);
		return (error);
	}
	init_coders(simulation);
	return (ERROR_NONE);
}

static t_simulation	*create_simulation(t_configuration config)
{
	t_simulation	*simulation;
	t_coder			*coders;
	t_dongle		*dongles;
	size_t			coders_number;

	simulation = malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->coders = NULL;
	simulation->dongles = NULL;
	coders_number = config.number_of_coders;
	coders = malloc(coders_number * sizeof(t_coder));
	if (!coders)
	{
		free(simulation);
		return (NULL);
	}
	dongles = malloc(coders_number * sizeof(t_dongle));
	if (!dongles)
		return (free(coders), free(simulation), NULL);
	simulation->config = config;
	simulation->dongles = dongles;
	simulation->coders = coders;
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
		destroy_simulation(simulation);
		return (NULL);
	}
	return (simulation);
}
