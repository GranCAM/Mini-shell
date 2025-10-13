/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:25:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 21:29:50 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* redirecciones en el padre (builtins) */
int	apply_redirs_parent(t_cmd *cmd, int *save_stdin, int *save_stdout)
{
	int	ok;

	ok = 1;
	*save_stdin = dup(STDIN_FILENO);
	*save_stdout = dup(STDOUT_FILENO);
	if (*save_stdin == -1 || *save_stdout == -1)
		return (0);
	if (cmd->redir.has_redir_in && cmd->redir.in_fd == -1)
	{
		cmd->skip_execution = 1;
		return (0);
	}
	apply_parent_out(cmd, &ok);
	apply_parent_in(cmd, &ok);
	if (!ok)
	{
		cmd->skip_execution = 1;
		return (0);
	}
	return (ok);
}

void	restore_redirs_parent(int save_stdin, int save_stdout, t_cmd *cmd)
{
	if (save_stdin != -1)
		dup2(save_stdin, STDIN_FILENO);
	if (save_stdout != -1)
		dup2(save_stdout, STDOUT_FILENO);
	if (save_stdin != -1)
		close(save_stdin);
	if (save_stdout != -1)
		close(save_stdout);
	(void)cmd;
}

static void	fill_cmd_array(t_exec_ctx *s, t_cmd *cmds)
{
	s->cur = cmds;
	s->i = 0;
	while (s->i < s->n)
	{
		s->cmd_arr[s->i] = s->cur;
		s->cur = s->cur->next;
		s->i++;
	}
}

int	init_ctx(t_exec_ctx *s, t_cmd *cmds)
{
	s->n = count_cmds(cmds);
	if (s->n == 0)
		return (0);
	s->pfd = NULL;
	s->pids = NULL;
	s->cmd_arr = NULL;
	if (alloc_exec_arrays(s) != 0)
		return (-1);
	fill_cmd_array(s, cmds);
	return (1);
}

/* orquestación delegada a executor_main.c */
int			executor_run(t_cmd *cmds, t_shell *shell);

int	execute_cmds(t_cmd *cmds, t_shell *shell)
{
	return (executor_run(cmds, shell));
}
