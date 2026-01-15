/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:45:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/08 17:39:29 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	apply_redirs_child(t_cmd *cmd, t_shell *shell)
{
	apply_in_redirs(cmd, shell);
	apply_out_redirs(cmd);
}

static void	exec_builtin_in_child(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		_exit(builtin_echo(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		_exit(builtin_cd(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		_exit(builtin_pwd());
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		_exit(builtin_export(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		_exit(builtin_unset(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		_exit(builtin_env(shell->envp));
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		_exit(builtin_exit(cmd->argv, shell->last_status));
}

void	exec_command_child(t_cmd *cmd, t_shell *shell, int in_pipeline)
{
	(void)in_pipeline;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	apply_redirs_child(cmd, shell);
	if (cmd->skip_execution)
		_exit(1);
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->argv && cmd->argv[0] && cmd->argv[0][0] == '\0')
		{
			ft_putstr_fd("'': command not found\n", 2);
		}
		_exit(127);
	}
	if (cmd->is_builtin)
		exec_builtin_in_child(cmd, shell);
	else if (ft_strchr(cmd->argv[0], '/'))
		exec_path_or_error(cmd, shell);
	else
		exec_search_in_path(cmd, shell);
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

void	fork_children(t_exec_ctx *s, t_shell *shell)
{
	s->ign.sa_handler = SIG_IGN;
	sigemptyset(&s->ign.sa_mask);
	s->ign.sa_flags = 0;
	sigaction(SIGINT, &s->ign, &s->old_int);
	s->i = s->n - 1;
	while (s->i >= 0)
	{
		s->node = s->cmd_arr[s->i];
		s->pids[s->i] = fork();
		if (s->pids[s->i] == -1)
			s->pids[s->i] = -1;
		else if (s->pids[s->i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGPIPE, SIG_DFL);
			dup_io_for_child(s);
			exec_command_child(s->node, shell, (s->n > 1));
			_exit(1);
		}
		else
			close_unused_fds(s);
		s->i--;
	}
}
