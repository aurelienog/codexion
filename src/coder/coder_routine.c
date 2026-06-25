/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:39 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 10:30:24 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void *single_coder_routine(void *arg)
{
    t_coder	*coder;

    coder = arg;
		print_status(coder, STATUS_TAKE_DONGLE);
    while (!simulation_finished(coder->simulation))
        usleep(1000);
    return (NULL);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	int		finished;
	int		required_compiles;

	coder = (t_coder *)arg;
	if (coder->simulation->config.number_of_coders == 1)
    return (single_coder_routine(coder));
	required_compiles = coder->simulation->config.number_of_compiles_required;
	pthread_mutex_lock(&coder->mutex);
	finished = coder->compiles_count >= required_compiles;
	pthread_mutex_unlock(&coder->mutex);
	while (!simulation_finished(coder->simulation)
		&& !finished)
	{
		request_compile(coder);
		if (simulation_finished(coder->simulation))
    	break;
		compile(coder);
		release_dongles(coder);
		// debug();
		// refactor();
	}
	return (NULL);
}
