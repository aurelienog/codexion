/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:04:49 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 12:59:36 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_positive_number(char *str)
{
	size_t	i;
	long	value;

	i = 0;
	value = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (value > (INT_MAX - (str[i] - '0')) / 10)
			return (0);
		value = value * 10 + (str[i] - '0');
		i++;
	}
	return (value > 0);
}

t_error	parse_input(int argc, char **argv)
{
	size_t	i;

	if (argc != 9)
		return (ERROR_ARGC);
	i = 1;
	while (i < 8)
	{
		if (!is_valid_positive_number(argv[i]))
			return (ERROR_VALUE);
		i++;
	}
	if ((strcmp(argv[8], "fifo") != 0)
		&& strcmp(argv[8], "edf") != 0)
		return (ERROR_SCHEDULER);
	return (ERROR_NONE);
}
