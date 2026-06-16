/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:00:40 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 12:47:52 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_configuration	create_config(char **argv)
{
	t_configuration	config;

	config.number_of_coders = atoi(argv[1]);
	config.time_to_burnout = atoi(argv[2]);
	config.time_to_compile = atoi(argv[3]);
	config.time_to_debug = atoi(argv[4]);
	config.time_to_refactor = atoi(argv[5]);
	config.number_of_compiles_required = atoi(argv[6]);
	config.dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		config.scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		config.scheduler = EDF;
	return (config);
}
