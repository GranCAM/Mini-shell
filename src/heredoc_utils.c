/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:24:19 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:30:36 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	hd_set_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

static void	hd_child(const char *delim, int no_expand, t_shell *shell,
		int pipefd[2])
{
	hd_set_child_signals();
	close(pipefd[0]);
	if (!read_heredoc_into_pipe(pipefd[1], delim, no_expand, shell))
	{
		close(pipefd[1]);
		_exit(130);
	}
	close(pipefd[1]);
	_exit(0);
}

int	hd_create_and_fill(const char *delim, int no_expand, t_shell *shell,
		int pipefd[2])
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (ms_perror("pipe"), 0);
	pid = fork();
	if (pid < 0)
		return (ms_perror("fork"), close(pipefd[0]), close(pipefd[1]), 0);
	if (pid == 0)
		hd_child(delim, no_expand, shell, pipefd);
	return (hd_parent_wait(pid, pipefd));
}

int	hd_process_list(t_cmd *cmd, t_shell *shell, int *assigned_fd)
{
	t_hdoc		*node;
	int			pipefd[2];
	const char	*delim;
	int			info[2];

	node = cmd->redir.heredocs;
	while (1)
	{
		hd_pick_source(node, cmd, &delim, info);
		if (!hd_create_and_fill(delim, info[0], shell, pipefd))
			return (hd_fail_cleanup(assigned_fd, -1, cmd));
		hd_update_assigned(assigned_fd, pipefd[0], info[1]);
		if (!node || !node->next)
			break ;
		node = node->next;
	}
	return (1);
}

int	hd_fail_cleanup(int *assigned_fd, int new_rfd, t_cmd *cmd)
{
	if (new_rfd != -1)
		close(new_rfd);
	if (*assigned_fd != -1)
		close(*assigned_fd);
	*assigned_fd = -1;
	cmd->skip_execution = 1;
	return (0);
}
