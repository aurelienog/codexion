/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:39 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 10:51:41 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void* execute_routine(void *arg)
{
    t_coder *coder;
    coder = (t_coder *)arg;
    while (!simulation_finished(coder->simulation))
    {
        request_left_dongle();
        request_right_dongle();

        compile();

        release_left();
        release_right();

        debug();

        refactor();
    }
}