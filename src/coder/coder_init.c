/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:46:04 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 11:33:47 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_error	init_coder(t_dongle *left_dongle,
		t_dongle *right_dongle,
		t_simulation *simulation,
		size_t i)
{
	t_coder	*coder;
	coder = &simulation->coders[i];
	if (pthread_mutex_init(&coder->mutex, NULL) != 0)
		return (ERROR_MUTEX);
	coder = &simulation->coders[i];
	coder->id = i + 1;
	coder->permission_to_compile = 0;
	coder->waiting_for_compile = 0;
	coder->compiles_count = 0;
	coder->last_compile_start = get_time_ms();
	coder->finished = 0;
	coder->simulation = simulation;
	coder->left = left_dongle;
	coder->right = right_dongle;
	return (ERROR_NONE);
}

t_error	init_coders(t_simulation *simulation)
{
	int			i;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		left_dongle = &simulation->dongles[i];
		if (i == simulation->config.number_of_coders - 1)
			right_dongle = &simulation->dongles[0];
		else
			right_dongle = &simulation->dongles[i + 1];
		if (init_coder(left_dongle, right_dongle, simulation, i) != ERROR_NONE)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&simulation->coders[i].mutex);
			}
			return (ERROR_MUTEX);
		}
		i += 1;
	}
	return (ERROR_NONE);
}
