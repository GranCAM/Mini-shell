/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:35:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 14:21:22 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	if (!argv || argv < (char **)0x1000)
		return ;
	i = 0;
	while (i < MAX_TOKENS && argv[i])
	{
		if (argv[i] > (char *)0x1000)
			free(argv[i]);
		i++;
	}
	free(argv);
}

static int	is_valid_cmd_ptr(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	/* Basic pointer validation */
	if (cmd < (t_cmd *)0x1000 || cmd > (t_cmd *)0x7fffffffffff)
		return (0);
	return (1);
}

static void	free_heredocs(t_hdoc *h)
{
	t_hdoc	*nx;
	int		safety_counter;

	safety_counter = 0;
	while (h && h > (t_hdoc *)0x1000 && safety_counter < 100)
	{
		nx = h->next;
		if (h->delim && h->delim > (char *)0x1000)
			free(h->delim);
		free(h);
		h = nx;
		safety_counter++;
	}
}

static void	close_redir_fds(t_redir *r)
{
	if (!r)
		return ;
	if (r->in_fd >= 0)
		close(r->in_fd);
	if (r->out_fd >= 0)
		close(r->out_fd);
	if (r->append_fd >= 0)
		close(r->append_fd);
	if (r->heredoc_fd >= 0)
		close(r->heredoc_fd);
}

static void	free_cmd_node(t_cmd *cmd)
{
	if (!cmd || !is_valid_cmd_ptr(cmd))
		return ;
	if (cmd->argv && cmd->argv > (char **)0x1000)
		free_argv(cmd->argv);
	if (cmd->redir.heredoc_delim && cmd->redir.heredoc_delim > (char *)0x1000)
		free(cmd->redir.heredoc_delim);
	if (cmd->redir.heredocs && cmd->redir.heredocs > (t_hdoc *)0x1000)
		free_heredocs(cmd->redir.heredocs);
	close_redir_fds(&cmd->redir);
	free(cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;
	int		safety_counter;

	safety_counter = 0;
	while (cmds && is_valid_cmd_ptr(cmds) && safety_counter < 1000)
	{
		next = cmds->next;
		if (is_valid_cmd_ptr(next) || !next)
			free_cmd_node(cmds);
		cmds = next;
		safety_counter++;
	}
}
