/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:26:37 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:26:40 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

#ifdef ECHOCTL

static void	set_echoctl_flag(struct termios *t)
{
	t->c_lflag |= ECHOCTL;
}
#else

static void	set_echoctl_flag(struct termios *t)
{
	(void)t;
}
#endif

static int	termios_keeper(int op, struct termios *io)
{
	static t_term_state	st;

	if (op == 1)
	{
		st.orig = *io;
		st.saved = 1;
		return (1);
	}
	if (op == 2)
	{
		if (!st.saved)
			return (0);
		*io = st.orig;
		return (1);
	}
	return (st.saved);
}

void	configure_terminal(void)
{
	struct termios	t;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &t) == -1)
		return ;
	if (!termios_keeper(0, NULL))
		termios_keeper(1, &t);
	set_echoctl_flag(&t);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	restore_terminal(void)
{
	struct termios	t;

	if (!termios_keeper(2, &t))
		return ;
	if (!isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
