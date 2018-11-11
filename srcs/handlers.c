/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keylogger.h"

int             shift_handler(int value, int state, int *modifier, int *capslock)
{
	if (state == 2)
		return 0;
	if (*capslock && (value == KEY_LEFTSHIFT || value == KEY_RIGHTSHIFT))
		state = !state;
	if (state == 0)
		*modifier -= (1 << value);
	else if (state == 1)
		*modifier += (1 << value);
	return 0;
}

int			lock_handler(int value, int state, int *modifier, int *capslock)
{
	if (state != 1)
		return 0;
	if (value == 6)
		*capslock = !(*capslock);
	if (*capslock == 0)
		*modifier -= (1 << KG_SHIFT);
	else
		*modifier += (1 << KG_SHIFT);
	return 0;
}