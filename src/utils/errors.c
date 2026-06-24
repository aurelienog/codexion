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
