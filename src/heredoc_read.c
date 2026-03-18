/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:35:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 19:33:19 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static char	*hd_read_prompt(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (ms_read_line_fd(STDIN_FILENO));
}

static int	hd_write_line(int wfd, char *line, int no_expand, t_shell *shell)
{
	char	*to_write;
	int		owns_to_write;

	to_write = line;
	owns_to_write = 0;
	if (!no_expand)
	{
		to_write = expand_env_var(line, shell->envp, shell->last_status);
		free(line);
		owns_to_write = 1;
	}
	write(wfd, to_write, ft_strlen(to_write));
	write(wfd, "\n", 1);
	if (owns_to_write)
		free(to_write);
	return (1);
}

int	read_heredoc_into_pipe(int wfd, const char *delim, int no_expand,
		t_shell *shell)
{
	char	*line;

	if (!delim || !*delim)
		return (0);
	while (1)
	{
		line = hd_read_prompt();
		if (!line)
			break ;
		if (g_signal == SIGINT)
		{
			free(line);
			return (0);
		}
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		hd_write_line(wfd, line, no_expand, shell);
	}
	return (1);
}

void	hd_pick_source(t_hdoc *node, t_cmd *cmd, const char **delim,
		int info[2])
{
	if (node)
	{
		*delim = node->delim;
		info[0] = node->no_expand;
		info[1] = (node->next == NULL);
	}
	else
	{
		*delim = cmd->redir.heredoc_delim;
		info[0] = cmd->redir.heredoc_no_expand;
		info[1] = 1;
	}
}
