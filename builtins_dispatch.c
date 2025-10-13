/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:19:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 11:19:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	dispatch_builtin_no_exit(t_shell *shell, char **av)
{
	if (ft_strncmp(av[0], "echo", 5) == 0)
		return (builtin_echo(av));
	else if (ft_strncmp(av[0], "cd", 3) == 0)
		return (builtin_cd(av, shell));
	else if (ft_strncmp(av[0], "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(av[0], "export", 7) == 0)
		return (builtin_export(av, shell));
	else if (ft_strncmp(av[0], "unset", 6) == 0)
		return (builtin_unset(av, shell));
	else if (ft_strncmp(av[0], "env", 4) == 0)
		return (builtin_env(shell->envp));
	return (-1);
}

static int	setup_redirs_and_check_skip(t_cmd *cmd, int *save_in, int *save_out)
{
	*save_in = -1;
	*save_out = -1;
	if (!apply_redirs_parent(cmd, save_in, save_out))
	{
		restore_redirs_parent(*save_in, *save_out, cmd);
		return (1);
	}
	return (0);
}

static void	perform_exit_and_quit(t_shell *shell, t_cmd *cmd, int save_in,
		int save_out)
{
	int	exit_code;

	exit_code = builtin_exit(cmd->argv, shell->last_status);
	restore_redirs_parent(save_in, save_out, cmd);
	free_cmds(cmd);
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 1);
	restore_terminal();
	free_env(shell->envp);
	close_extra_fds();
	exit(exit_code);
}

static int	pre_checks_and_redirs(t_shell *shell, t_cmd *cmd, int *save_in,
		int *save_out)
{
	if (cmd->skip_execution)
		return (shell->last_status = 1);
	if (cmd->redir.has_redir_in && cmd->redir.in_fd == -1)
		return (shell->last_status = 1);
	if (setup_redirs_and_check_skip(cmd, save_in, save_out))
	{
		if (cmd->skip_execution)
			shell->last_status = 1;
		return (shell->last_status);
	}
	return (-42);
}

int	run_single_builtin(t_shell *shell, t_cmd *cmd, int *save_in, int *save_out)
{
	char	**av;
	int		status;
	int		pre;

	av = cmd->argv;
	pre = pre_checks_and_redirs(shell, cmd, save_in, save_out);
	if (pre != -42)
		return (pre);
	if (ft_strncmp(av[0], "exit", 5) == 0)
		perform_exit_and_quit(shell, cmd, *save_in, *save_out);
	status = dispatch_builtin_no_exit(shell, av);
	if (status >= 0)
		shell->last_status = status;
	restore_redirs_parent(*save_in, *save_out, cmd);
	return (shell->last_status);
}
