#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
	
static int			is_a_console(int fd)
{
	char			arg;

	arg = 0;
	return (ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84)));
}

int					get_console()
{
	char			*console[] = {
		"/proc/self/fd/0",
		"/dev/tty",
		"/dev/tty0",
		"/dev/vc/0",
		"/dev/systty",
		"/dev/console"
	};
	int				fd;

	fd = -1;
	for (size_t i = 0; i < sizeof(console) / sizeof(*console); i++)
	{
		fd = open(console[i], O_RDWR);
		if (fd < 0 && errno == EACCES)
			fd = open(console[i], O_WRONLY);
		if (fd < 0 && errno == EACCES)
			fd = open(console[i], O_RDONLY);
		if (fd < 0)
			continue ;
		if (is_a_console(fd))
		{
			fprintf(stdout, "Console: %s\n", console[i]);
			return fd;
		}
		close(fd);
		fd = -1;
	}
	if (fd < 0)
		for (fd = 0; fd < 3; fd++)
			if (is_a_console(fd))
				return fd;
	fprintf(stderr, "Can't get a file descriptor refering to a terminal\n");
	fprintf(stderr, "Are you r00t ?\n");
	return -1;
}
