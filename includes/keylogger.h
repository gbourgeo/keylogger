/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylogger.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYLOGGER_H
# define KEYLOGGER_H

#include <linux/input.h>
#include <linux/keyboard.h>

typedef struct 	s_modifiers
{
	int 		value;
	int 		bit;
	char 		*name;
}				t_modifiers;

static t_modifiers		modifiers[] = {
	{ 0, 				KG_SHIFT,		"shift"},
	{ KEY_RIGHTALT, 	KG_ALTGR,		"altgr"},
	{ 0, 				KG_CTRL,		"control"},
	{ KEY_LEFTALT, 		KG_ALT,			"alt"},
	{ KEY_LEFTSHIFT, 	KG_SHIFTL,		"shiftl"},
	{ KEY_RIGHTSHIFT, 	KG_SHIFTR,		"shiftr"},
	{ KEY_LEFTCTRL, 	KG_CTRLL,		"ctrll"},
	{ KEY_RIGHTCTRL, 	KG_CTRLR,		"ctrlr"}
};

static int				loop = 1;

#endif
