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

static int cooldown_may_have_expired(t_simulation *simulation)
{
    int      i;
    long long   now;
    int         changed;
    t_dongle    *dongle;

    now = get_time_ms();
    changed = 0;
    i = 0;
    while (i < simulation->config.number_of_coders)
    {
        dongle = &simulation->dongles[i];
        pthread_mutex_lock(&dongle->mutex);
        if (dongle->release_time != 0
            && !dongle->cooldown_expired_notified
            && now >= dongle->release_time
                + simulation->config.dongle_cooldown)
        {
            dongle->cooldown_expired_notified = 1;
            changed = 1;
        }
        pthread_mutex_unlock(&dongle->mutex);
        i++;
    }
    return (changed);
}

void	*watch(void *arg)
{
	t_monitor	*monitor;
	t_simulation *simulation;

	monitor = (t_monitor *)arg;
	simulation = monitor->simulation;
	while (!simulation_finished(simulation))
	{
		check_burnout(simulation);
		check_completion(simulation);
		if (cooldown_may_have_expired(simulation))
    {
        pthread_mutex_lock(&simulation->scheduler_mutex);
        pthread_cond_broadcast(&simulation->scheduler_cond);
        pthread_mutex_unlock(&simulation->scheduler_mutex);
    }
		usleep(1000);
	}
	return (NULL);
}
