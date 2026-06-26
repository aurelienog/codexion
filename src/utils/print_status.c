/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 12:23:18 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/26 12:24:17 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, t_status status)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->simulation->print_mutex);
	if (simulation_finished(coder->simulation)
		&& status != STATUS_BURNED
		&& status != STATUS_FINISHED)
	{
		pthread_mutex_unlock(&coder->simulation->print_mutex);
		return ;
	}
	timestamp = get_time_ms() - coder->simulation->start_time;
	printf("%lld %d ", timestamp, coder->id);
	if (status == STATUS_COMPILING)
		printf("is compiling\n");
	else if (status == STATUS_DEBUGGING)
		printf("is debugging\n");
	else if (status == STATUS_REFACTORING)
		printf("is refactoring\n");
	else if (status == STATUS_TAKE_DONGLE)
		printf("has taken a dongle\n");
	else if (status == STATUS_BURNED)
		printf("burned out\n");
	pthread_mutex_unlock(&coder->simulation->print_mutex);
}
