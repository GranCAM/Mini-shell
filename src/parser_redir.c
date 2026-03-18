/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:14 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:25:16 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	parse_redir(t_parse_ctx *c, int *argc, char ***argv, int *parse_error)
{
	c->redir_type = classify_redir_and_advance(c, &c->append, &c->is_heredoc);
	if (c->redir_type == '\0')
	{
		ms_syntax_error("<");
		*parse_error = 1;
		return ;
	}
	c->filename_noq = get_filename_noq(c, c->is_heredoc, &c->no_expand_heredoc,
			parse_error);
	if (!c->filename_noq)
		return ;
	attach_new_cmd_if_needed(c, argc, argv);
	if ((*c->current_cmd)->skip_execution)
	{
		free(c->filename_noq);
		c->filename_noq = NULL;
		return ;
	}
	if (c->redir_type == '>')
		redir_output(*c->current_cmd, c->filename_noq, c->append);
	else
		redir_input_or_heredoc(*c->current_cmd, c->filename_noq, c->is_heredoc,
			c->no_expand_heredoc);
	free(c->filename_noq);
	c->filename_noq = NULL;
}

int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	hd_parent_wait(pid_t pid, int pipefd[2])
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

int	hd_handle_child_status(int status, int rfd)
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
