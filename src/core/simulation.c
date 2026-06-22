/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:52:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 15:45:56 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_simulation(t_simulation *simulation)
{
	size_t	i;

	if (!simulation)
		return ;
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		free(simulation->dongles[i].queue.data);
		// pthread_cond_destroy(&simulation->dongles[i].cond);
		//pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->state_mutex);
	pthread_mutex_destroy(&simulation->scheduler.mutex);
	pthread_cond_destroy(&simulation->scheduler.cond);
	free(simulation->dongles);
	free(simulation->coders);
	free(simulation);
}

static t_error	init_simulation(t_simulation *simulation)
{
	t_error	error;

	if (pthread_cond_init(&simulation->scheduler.cond, NULL) != 0)
		return (ERROR_MUTEX);
	if (pthread_mutex_init(&simulation->state_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		return (ERROR_MUTEX);
	}
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		pthread_mutex_destroy(&simulation->state_mutex);
		return (ERROR_MUTEX);
	}
	if (pthread_mutex_init(&simulation->scheduler.mutex, NULL) != 0)
	{
		pthread_cond_destroy(&simulation->scheduler.cond);
		pthread_mutex_destroy(&simulation->print_mutex);
		pthread_mutex_destroy(&simulation->state_mutex);
		return (ERROR_MUTEX);
	}
	simulation->created_coders = 0;
	simulation->request_counter = 0;
	simulation->termination_flag = 0;
	simulation->start_time = get_time_ms();
	error = init_dongles(simulation);
	if (error != ERROR_NONE)
	{
		pthread_mutex_destroy(&simulation->print_mutex);
		pthread_mutex_destroy(&simulation->state_mutex);
		pthread_mutex_destroy(&simulation->scheduler.mutex);
		pthread_cond_destroy(&simulation->scheduler.cond);
		return (error);
	}
	init_coders(simulation);
	return (ERROR_NONE);
}

static t_simulation	*create_simulation(t_configuration config)
{
	t_simulation	*simulation;
	t_scheduler		scheduler;
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
	simulation->scheduler = scheduler;

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
		free(simulation);
		return (NULL);
	}
	return (simulation);
}
