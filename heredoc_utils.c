/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:55:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 20:22:26 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* forward static prototypes */
static int			hd_parent_wait(pid_t pid, int pipefd[2]);
static int			hd_fail_cleanup(int *assigned_fd, int new_rfd, t_cmd *cmd);
static int			hd_handle_child_status(int status, int rfd);
static int			hd_update_assigned(int *assigned_fd, int rfd, int is_last);

static int	hd_create_and_fill(const char *delim, int no_expand, t_shell *shell,
		int pipefd[2])
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (ms_perror("pipe"), 0);
	pid = fork();
	if (pid < 0)
	{
		ms_perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		close(pipefd[0]);
		if (!read_heredoc_into_pipe(pipefd[1], delim, no_expand, shell))
		{
			close(pipefd[1]);
			_exit(130);
		}
		close(pipefd[1]);
		_exit(0);
	}
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

static int	hd_fail_cleanup(int *assigned_fd, int new_rfd, t_cmd *cmd)
{
	if (new_rfd != -1)
		close(new_rfd);
	if (*assigned_fd != -1)
		close(*assigned_fd);
	*assigned_fd = -1;
	cmd->skip_execution = 1;
	return (0);
}

static int	hd_handle_child_status(int status, int rfd)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		g_signal = SIGINT;
		close(rfd);
		return (0);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(rfd);
		return (0);
	}
	return (1);
}

static int	hd_parent_wait(pid_t pid, int pipefd[2])
{
	struct sigaction	ign;
	struct sigaction	old_int;
	int					status;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	close(pipefd[1]);
	if (waitpid(pid, &status, 0) < 0)
	{
		ms_perror("waitpid");
		sigaction(SIGINT, &old_int, NULL);
		close(pipefd[0]);
		return (0);
	}
	sigaction(SIGINT, &old_int, NULL);
	return (hd_handle_child_status(status, pipefd[0]));
}

static int	hd_update_assigned(int *assigned_fd, int rfd, int is_last)
{
	if (is_last)
	{
		if (*assigned_fd != -1)
			close(*assigned_fd);
		*assigned_fd = rfd;
	}
	else
		close(rfd);
	return (1);
}

