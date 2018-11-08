/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_keymaps.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <linux/keyboard.h>
#include <linux/kd.h>
#include <errno.h>
#include <stdio.h>

#ifndef MAX_NR_KEYMAPS
# define MAX_NR_KEYMAPS NR_KEYMAPS
#endif

int					get_keymaps(int fd, int *keymaps[256])
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
			fprintf(stderr, "KDBGENT error at index 0 in table %d\n", i);
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
