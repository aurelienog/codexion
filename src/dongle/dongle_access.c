/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquire.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aunoguei <aunoguei@student.42urduliz.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:37:55 by aunoguei          #+#    #+#             */
/*   Updated: 2026/06/23 15:13:32 by aunoguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left < coder->right)
	{
		first = coder->left ;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left ;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
}

void	unlock_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left < coder->right)
	{
		first = coder->left ;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left ;
	}
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
}

void	take_dongles(t_coder *coder)
{
	lock_both_dongles(coder);
	coder->left->is_available = 0;
	coder->right->is_available = 0;
	unlock_both_dongles(coder);
}

void	release_dongles(t_coder *coder)
{
	long long	now;

	now = get_time_ms();
	lock_both_dongles(coder);
	coder->left->is_available = 1;
	coder->left->cooldown_expired_notified = 0;
	coder->left->release_time = now;
	coder->right->is_available = 1;
	coder->right->cooldown_expired_notified = 0;
	coder->right->release_time = now;
	unlock_both_dongles(coder);
}
