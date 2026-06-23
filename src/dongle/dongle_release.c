/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_release.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:39:03 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/23 14:10:24 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongles(t_coder *coder)
{
	lock_both_dongles(coder);
	coder->left->is_available = 1;
	coder->right->is_available = 1;
	unlock_both_dongles(coder);
	pthread_mutex_lock(&coder->simulation->scheduler_mutex);
	pthread_cond_broadcast(&coder->simulation->scheduler_cond);
	pthread_mutex_unlock(&coder->simulation->scheduler_mutex);
}
