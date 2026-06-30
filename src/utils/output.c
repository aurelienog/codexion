/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:48:47 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 13:30:25 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_error(t_error error)
{
	if (error == ERROR_ARGC)
		printf("Error: invalid number of arguments\n");
	else if (error == ERROR_VALUE)
		printf("Error: invalid number\n");
	else if (error == ERROR_SCHEDULER)
		printf("Error: invalid scheduler (fifo or edf only)\n");
}

void	print_usage(void)
{
	printf("[INPUT ERROR]: the following arguments are mandatory:\n");
	printf("- number_of_coder\n");
	printf("- time_to_burnout\n");
	printf("- time_to_compile\n");
	printf("- time_to_debug\n");
	printf("- time_to_refactor\n");
	printf("- number_of_compiles_required\n");
	printf("- dongle_cooldown\n");
	printf("- scheduler [fifo || edf]\n");
}

void	print_status(t_coder *coder, t_status status)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->simulation->print_mutex);
	if (simulation_finished(coder->simulation) && status != STATUS_BURNED
		&& status != STATUS_FINISHED)
	{
		pthread_mutex_unlock(&coder->simulation->print_mutex);
		return ;
	}
	timestamp = get_time_ms() - coder->simulation->start_time;
	if (status == STATUS_COMPILING)
	{
		printf("%lld %d has taken a dongle\n", timestamp, coder->id);
		printf("%lld %d has taken a dongle\n", timestamp, coder->id);
		printf("%lld %d is compiling\n", timestamp, coder->id);
	}
	else if (status == STATUS_DEBUGGING)
		printf("%lld %d is debugging\n", timestamp, coder->id);
	else if (status == STATUS_REFACTORING)
		printf("%lld %d is refactoring\n", timestamp, coder->id);
	else if (status == STATUS_TAKE_DONGLE)
		printf("%lld %d has taken a dongle\n", timestamp, coder->id);
	else if (status == STATUS_BURNED)
		printf("%lld %d burned out\n", timestamp, coder->id);
	pthread_mutex_unlock(&coder->simulation->print_mutex);
}
