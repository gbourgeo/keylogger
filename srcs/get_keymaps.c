#include <sys/ioctl.h>
#include <linux/keyboard.h>
#include <linux/kd.h>
#include <errno.h>
#include <stdio.h>

#include "main.h"

int					get_keymaps(int fd, int **keymaps)
{
	int 			keymapnbr;
	struct kbentry	ke;

	keymapnbr = 0;
	for (int i = 0; i < MAX_NR_KEYMAPS; i++)	
	{
		ke.kb_index = 0;
		ke.kb_table = i;
		int j = ioctl(fd, KDGKBENT, (unsigned long)&ke);
		if (j && errno != EINVAL) {
			perror("KDGKBENT");
			fprintf(stderr, "KDGKBENT: error at index 0 in table %d\n", i);
			return 0;
		}
		if (!j && ke.kb_value != K_NOSUCHMAP) {
			keymaps[0][i] = keymapnbr;
			keymaps[1][keymapnbr++] = i;
		} else {
			keymaps[0][i] = -1;
		}
	}
	if (keymapnbr == 0)
		fprintf(stderr, "cannot find any keymaps\n");
	if (keymaps[1][0] != 0)
		fprintf(stderr, "plain map not alocated !? o_O ?!");
	return keymapnbr;
}
