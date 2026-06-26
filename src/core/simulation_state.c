/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:20:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:46:55 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_simulation(t_simulation *simulation)
{
	int	i;

	if (!simulation)
		return ;
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		free(simulation->dongles[i].queue.data);
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		pthread_mutex_destroy(&simulation->coders[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->state_mutex);
	pthread_mutex_destroy(&simulation->scheduler_mutex);
	pthread_cond_destroy(&simulation->scheduler_cond);
	free(simulation->dongles);
	free(simulation->coders);
	free(simulation);
}

void	set_simulation_finished(t_simulation *simulation)
{
	int	already_finished;

	pthread_mutex_lock(&simulation->state_mutex);
	already_finished = simulation->termination_flag;
	if (!already_finished)
		simulation->termination_flag = 1;
	pthread_mutex_unlock(&simulation->state_mutex);
	if (!already_finished)
	{
		pthread_mutex_lock(&simulation->scheduler_mutex);
		pthread_cond_broadcast(&simulation->scheduler_cond);
		pthread_mutex_unlock(&simulation->scheduler_mutex);
	}
}

int	simulation_finished(t_simulation *simulation)
{
	int	is_over;

	pthread_mutex_lock(&simulation->state_mutex);
	is_over = simulation->termination_flag;
	pthread_mutex_unlock(&simulation->state_mutex);
	return (is_over);
}
