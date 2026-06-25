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
