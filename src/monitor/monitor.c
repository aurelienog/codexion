/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:39:52 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/22 10:45:37 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*watch(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (!simulation_finished(monitor->simulation))
	{
		check_burnout(monitor->simulation);
		check_completion(monitor->simulation);
		usleep(500);
	}
	return (NULL);
}
