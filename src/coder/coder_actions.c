/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:20 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 10:51:55 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	print_status(coder, STATUS_COMPILING);
	usleep(coder->simulation->config.time_to_compile * 1000);
	pthread_mutex_lock(&coder->mutex);
	coder->compiles_count++;
	if (coder->compiles_count
		>= coder->simulation->config.number_of_compiles_required)
		coder->finished = 1;
	pthread_mutex_unlock(&coder->mutex);
}

void	debug(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	print_status(coder, STATUS_DEBUGGING);
	usleep(coder->simulation->config.time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	if (simulation_finished(coder->simulation))
		return ;
	print_status(coder, STATUS_REFACTORING);
	usleep(coder->simulation->config.time_to_refactor * 1000);
}
