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

void	*routine(void *arg)
{
	t_coder	*coder;
	int		required_compiles;

	coder = (t_coder *)arg;
	required_compiles = coder->simulation->config.number_of_compiles_required;
	coder = (t_coder *)arg;
	while (!simulation_finished(coder->simulation)
		&& coder->compiles_count < required_compiles)
	{
		request_compile(coder);
		compile(coder);
		release_dongles(coder);
		// debug();
		// refactor();
	}
	return (NULL);
}
