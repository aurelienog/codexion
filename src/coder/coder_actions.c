/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:51:20 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/16 10:51:23 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

    // bool    is_compiling;
    // bool    is_refactoring;
    // bool    is_debugging;
    //request_dongle()
    // compile():
        // is_compiling = True
        //
        //replace_dongles()
        //is_compiling = False
        // debug()

    // debug():
        //is_debugging = True
        //
        //is_debugging = False
        //refactor()


    // refactor():
        //is_refactoring = True
        //
        //is_refactoring = False
void    request_compile(t_coder *coder)
{
    int is_left_available;
    int is_right_available;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;

    left_dongle = coder->left;
    right_dongle = coder->right;
    is_left_available = 0;
    is_right_available = 0;
    if (pthread_mutex_lock(&left_dongle->mutex) != 0)
        return (ERROR_MUTEX);
    if (pthread_mutex_lock(&right_dongle->mutex) != 0)
    {
        pthread_mutex_unlock(&left_dongle->mutex)
        return (ERROR_MUTEX);
    }

    if (left_dongle->is_available && is_next(left_dongle, coder))
        is_left_available = 1;

    if (right_dongle->is_available && is_next(right_dongle, coder))
        is_right_available = 1;

    if (is_left_available && is_right_available)
    {
        if (pthread_mutex_lock(coder->simulation) != 0)
        {
                pthread_mutex_unlock(&left_dongle->mutex);
                pthread_mutex_unlock(&right_dongle->mutex->mutex);
                return ;
        }
        take_dongles();
        left->is_available = 0;
        right->is_available = 0;
    }
    else
    {
        enqueue_request(t_dongle *dongle, t_request request);
        wait();
    }
    pthread_mutex_unlock(&left_dongle->mutex);
    pthread_mutex_unlock(&right_dongle->mutex);
    pthread_mutex_unlock(&coder->simulation->mutex);
}
    
    release_dongles();
    debug()
    refactor()