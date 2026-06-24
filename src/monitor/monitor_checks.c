/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:41:33 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/24 13:28:33 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_burnout(t_simulation *simulation)
{
	int			i;
	long long	elapsed;
	long long	last_compile;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].mutex);
		last_compile = simulation->coders[i].last_compile_start;
		pthread_mutex_unlock(&simulation->coders[i].mutex);
		elapsed = get_time_ms() - last_compile;
		if (elapsed >= simulation->config.time_to_burnout)
		{
			set_simulation_finished(simulation);
				pthread_mutex_lock(&simulation->print_mutex);
				printf("coder: %d burned", simulation->coders[i].id);
				pthread_mutex_unlock(&simulation->print_mutex);
			return ;
		}
		i++;
	}
	return ;
}

void	check_completion(t_simulation *simulation)
{
	int		i;
	int		finished;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].mutex);
		finished = simulation->coders[i].finished;
		pthread_mutex_unlock(&simulation->coders[i].mutex);
		if (!finished)
			return ;
		i++;
	}
	set_simulation_finished(simulation);
}
