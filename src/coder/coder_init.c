/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:46:04 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/23 15:14:13 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_error	init_coder(t_dongle *left_dongle,
		t_dongle *right_dongle,
		t_simulation *simulation,
		size_t i)
{
	t_coder	*coder;
	if (pthread_mutex_init(&coder->mutex, NULL) != 0)
		return (ERROR_MUTEX);
	coder = &simulation->coders[i];
	coder->id = i + 1;
	coder->compiles_count = 0;
	coder->last_compile_start = simulation->start_time;
	coder->finished = 0;
	coder->simulation = simulation;
	coder->left = left_dongle;
	coder->right = right_dongle;
	return (ERROR_NONE);
}

t_error	init_coders(t_simulation *simulation)
{
	size_t		i;
	size_t		coders_number;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_error		error;

	i = 0;
	coders_number = simulation->config.number_of_coders;
	while (i < coders_number)
	{
		left_dongle = &simulation->dongles[i];
		if (i == coders_number - 1)
			right_dongle = &simulation->dongles[0];
		else
			right_dongle = &simulation->dongles[i + 1];
		error = init_coder(left_dongle, right_dongle, simulation, i);
		if ( error != ERROR_NONE)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&simulation->coders[i].mutex);
			}
			return (error);
		}
		i += 1;
	}
	return (ERROR_NONE);
}
