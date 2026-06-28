/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:39 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 11:36:42 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*single_coder_routine(void *arg)
{
	t_coder	*coder;

	coder = arg;
	print_status(coder, STATUS_TAKE_DONGLE);
	while (!simulation_finished(coder->simulation))
		usleep(1000);
	return (NULL);
}

static int	has_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->mutex);
	finished = coder->finished;
	pthread_mutex_unlock(&coder->mutex);
	return (finished);
}

static void	release_compilation_resources(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->scheduler.mutex);
	release_dongles(coder);
	pthread_mutex_unlock(&coder->simulation->scheduler.mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->simulation->config.number_of_coders == 1)
		return (single_coder_routine(coder));
	while (!simulation_finished(coder->simulation))
	{
		if (has_finished(coder))
			break ;
		request_compile(coder);
		if (simulation_finished(coder->simulation))
			break ;
		compile(coder);
		release_compilation_resources(coder);
		if (has_finished(coder))
			break ;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
