/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:18:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/14 17:02:10 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

static char	*read_input_tty(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			shell->last_status = 130;
			if (!line)
				return (NULL);
		}
		break ;
	}
	if (line && *line && !shell->in_heredoc)
		add_history(line);
	return (line);
}

static char	*read_input_non_tty(void)
{
	static int	printed;

	if (!printed)
	{
		write(STDOUT_FILENO, "minishell$\n", 11);
		printed = 1;
	}
	return (ms_read_line_fd(STDIN_FILENO));
}

char	*read_input(t_shell *shell)
{
	char	*result;

	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		result = read_input_tty(shell);
		return (result);
	}
	return (read_input_non_tty());
}

int	should_print_bpipe(t_exec_ctx *s, int idx)
{
	t_cmd	*writer;
	t_cmd	*next;

	if (s->n != 2)
		return (0);
	if (!(WIFSIGNALED(s->status) && WTERMSIG(s->status) == SIGPIPE))
		return (0);
	writer = s->cmd_arr[idx];
	next = s->cmd_arr[idx + 1];
	if (!writer || writer->is_builtin)
		return (0);
	if (next && (next->redir.has_redir_in || next->redir.in_fd > 0
			|| next->redir.heredoc_fd >= 0 || next->redir.heredoc_fd == -2))
		return (0);
	return (1);
}
