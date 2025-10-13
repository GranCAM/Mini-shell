/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:44:02 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/13 18:47:44 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

int		g_signal = 0;

void	sig_handler(int signo)
{
	g_signal = signo;
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

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

int	setup_pipes(t_exec_ctx *s)
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

void	wait_others(t_exec_ctx *s)
{
	int	printed;

	printed = 0;
	s->k = 0;
	while (s->k < s->n - 1)
	{
		if (s->pids[s->k] > 0)
		{
			waitpid(s->pids[s->k], &s->status, 0);
			if (!printed && should_print_bpipe(s, s->k))
			{
				ft_putstr_fd(" Broken pipe\n", 2);
				printed = 1;
			}
		}
		s->k++;
	}
}

int	wait_and_finalize(t_exec_ctx *s, t_shell *shell)
{
	if (s->pids[s->n - 1] > 0)
		waitpid(s->pids[s->n - 1], &s->last_status, 0);
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
