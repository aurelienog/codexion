/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:20:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 10:22:03 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_simulation_finished(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->state_mutex);
	if (!simulation->termination_flag)
		simulation->termination_flag = 1;
	pthread_mutex_unlock(&simulation->state_mutex);
	pthread_mutex_lock(&simulation->scheduler_mutex);
	pthread_cond_broadcast(&simulation->scheduler_cond);
	pthread_mutex_lock(&simulation->scheduler_mutex);
}

int	simulation_finished(t_simulation *simulation)
{
	int	is_over;

	pthread_mutex_lock(&simulation->state_mutex);
	is_over = simulation->termination_flag;
	pthread_mutex_unlock(&simulation->state_mutex);
	return (is_over);
}
