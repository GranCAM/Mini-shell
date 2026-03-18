/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:22:19 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:30:46 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	cleanup_pipes_on_error(t_exec_ctx *s, int i)
{
	while (i-- > 0)
	{
		if (s->pfd[i][0] >= 0)
		{
			close(s->pfd[i][0]);
			s->pfd[i][0] = -1;
		}
		if (s->pfd[i][1] >= 0)
		{
			close(s->pfd[i][1]);
			s->pfd[i][1] = -1;
		}
	}
}

static int	setup_pipes(t_exec_ctx *s)
{
	int	i;

	if (s->n <= 1)
		return (1);
	i = 0;
	while (i < s->n - 1)
	{
		if (pipe(s->pfd[i]) == -1)
		{
			ms_perror("pipe");
			cleanup_pipes_on_error(s, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	wait_others(t_exec_ctx *s)
{
	int	printed;

	printed = 0;
	s->k = 0;
	while (s->k < s->n - 1)
	{
		if (s->pids[s->k] > 0)
		{
			waitpid(s->pids[s->k], &s->status, 0);
			if (!printed && WIFSIGNALED(s->status)
				&& WTERMSIG(s->status) == SIGPIPE)
			{
				ft_putstr_fd(" Broken pipe\n", 2);
				printed = 1;
			}
		}
		s->k++;
	}
}

static int	wait_and_finalize(t_exec_ctx *s, t_shell *shell)
{
	int	sigint_detected;

	sigint_detected = 0;
	if (s->pids[s->n - 1] > 0)
		waitpid(s->pids[s->n - 1], &s->last_status, 0);
	if (WIFSIGNALED(s->last_status) && WTERMSIG(s->last_status) == SIGINT)
	{
		sigint_detected = 1;
	}
	wait_others(s);
	sigaction(SIGINT, &s->old_int, NULL);
	if (WIFEXITED(s->last_status))
		shell->last_status = WEXITSTATUS(s->last_status);
	else if (WIFSIGNALED(s->last_status))
		shell->last_status = 128 + WTERMSIG(s->last_status);
	else
		shell->last_status = 1;
	return (shell->last_status);
}

int	executor_run(t_cmd *cmds, t_shell *shell)
{
	t_exec_ctx	s;
	int			res;

	res = init_ctx(&s, cmds);
	if (res <= 0)
		return (res < 0);
	if (!setup_pipes(&s))
	{
		free(s.pids);
		free(s.pfd);
		free(s.cmd_arr);
		return (1);
	}
	fork_children(&s, shell);
	if (s.n > 1)
		close_all_pipes(s.n, s.pfd);
	res = wait_and_finalize(&s, shell);
	free(s.pids);
	free(s.pfd);
	free(s.cmd_arr);
	return (res);
}
