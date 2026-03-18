/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:38 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:25:41 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	exec_after_parsing(t_shell *shell, t_loop_ctx *c)
{
	if (!shell->parser.cmds->next)
	{
		if (shell->parser.cmds->is_builtin)
		{
			if (should_skip_single(shell->parser.cmds))
				shell->last_status = 1;
			else
				shell->last_status = run_single_builtin(shell,
						shell->parser.cmds, &c->save_in, &c->save_out);
		}
		else if (should_skip_single(shell->parser.cmds))
			shell->last_status = 1;
		else
			shell->last_status = execute_cmds(shell->parser.cmds, shell);
	}
	else
		shell->last_status = execute_cmds(shell->parser.cmds, shell);
}

void	process_line(t_shell *shell, t_loop_ctx *c)
{
	if (!c->input)
		return ;
	shell->parser.cmds = parse_input(c->input, shell->envp, shell->last_status);
	if (!shell->parser.cmds)
		return (free(c->input), (void)0);
	free(c->input);
	c->input = NULL;
	if (!shell->parser.cmds->next && shell->parser.cmds->redir.has_redir_in
		&& shell->parser.cmds->redir.in_fd == -1)
		shell->last_status = 1;
	preprocess_heredocs(shell->parser.cmds, shell);
	if (g_signal == SIGINT)
	{
		shell->last_status = 130;
		g_signal = 0;
		free_cmds(shell->parser.cmds);
		shell->parser.cmds = NULL;
		return ;
	}
	exec_after_parsing(shell, c);
	if (shell->last_status == 130)
		write(2, "\n", 1);
	free_cmds(shell->parser.cmds);
	shell->parser.cmds = NULL;
}

static void	init_shell_defaults(t_shell *shell)
{
	shell->in_heredoc = 0;
	shell->parser.input = NULL;
	shell->parser.cmds = NULL;
	shell->parser.pipe_count = 0;
	shell->last_status = 0;
}

void	init_shell_state(t_shell *shell, struct sigaction *sa_pipe,
		t_loop_ctx *c, char **envp)
{
	c->save_in = -1;
	c->save_out = -1;
	shell->envp = env_dup(envp);
	init_shell_defaults(shell);
	sa_pipe->sa_handler = SIG_IGN;
	sigemptyset(&sa_pipe->sa_mask);
	sa_pipe->sa_flags = 0;
	sigaction(SIGPIPE, sa_pipe, NULL);
	setup_signals();
	configure_terminal();
}

int	handle_blank_input(t_loop_ctx *c)
{
	if (is_blank_str(c->input) && isatty(STDIN_FILENO))
	{
		free(c->input);
		c->input = NULL;
		return (1);
	}
	return (0);
}
