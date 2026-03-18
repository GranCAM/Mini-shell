/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 21:29:50 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	heredoc_sigint(int signo)
{
	char	nl;

	(void)signo;
	g_signal = SIGINT;
	if (isatty(STDIN_FILENO))
	{
		nl = '\n';
		ioctl(STDIN_FILENO, TIOCSTI, &nl);
	}
	else
		write(STDOUT_FILENO, "\n", 1);
}

void	handle_heredoc(t_cmd *cmd, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	new_int;
	int					assigned_fd;

	if (!(cmd->redir.heredoc_fd == -2))
		return ;
	if (!cmd->redir.heredocs && !cmd->redir.heredoc_delim)
		return ;
	new_int.sa_handler = heredoc_sigint;
	sigemptyset(&new_int.sa_mask);
	new_int.sa_flags = 0;
	sigaction(SIGINT, &new_int, &old_int);
	assigned_fd = -1;
	if (!hd_process_list(cmd, shell, &assigned_fd))
		cmd->redir.heredoc_fd = -1;
	else
		cmd->redir.heredoc_fd = assigned_fd;
	sigaction(SIGINT, &old_int, NULL);
}

void	preprocess_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*cur;

	cur = cmds;
	shell->in_heredoc = 1;
	while (cur)
	{
		if (!cur->skip_execution && cur->redir.heredoc_fd == -2
			&& (cur->redir.heredocs || cur->redir.heredoc_delim))
			handle_heredoc(cur, shell);
		cur = cur->next;
	}
	shell->in_heredoc = 0;
}

void	apply_parent_out(t_cmd *cmd, int *ok)
{
	if (cmd->redir.out_fd != -1)
	{
		if (dup2(cmd->redir.out_fd, STDOUT_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.out_fd);
		cmd->redir.out_fd = -1;
	}
	if (cmd->redir.append_fd != -1)
	{
		if (dup2(cmd->redir.append_fd, STDOUT_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.append_fd);
		cmd->redir.append_fd = -1;
	}
}

void	apply_parent_in(t_cmd *cmd, int *ok)
{
	if (cmd->redir.in_fd != -1)
	{
		if (dup2(cmd->redir.in_fd, STDIN_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.in_fd);
		cmd->redir.in_fd = -1;
	}
	if (cmd->redir.heredoc_fd >= 0)
	{
		if (dup2(cmd->redir.heredoc_fd, STDIN_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.heredoc_fd);
		cmd->redir.heredoc_fd = -1;
	}
}
