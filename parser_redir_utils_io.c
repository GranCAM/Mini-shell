/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils_io.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:24:58 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:25:01 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	attach_new_cmd_if_needed(t_parse_ctx *c, int *argc, char ***argv)
{
	int	k;

	if (!*c->current_cmd)
	{
		(*argv)[*argc] = NULL;
		*c->cmds = add_cmd(*c->cmds, *argv);
		k = 0;
		while (k < *argc)
		{
			free((*argv)[k]);
			k++;
		}
		free(*argv);
		*c->current_cmd = *c->cmds;
		while (*c->current_cmd && (*c->current_cmd)->next)
			*c->current_cmd = (*c->current_cmd)->next;
		*argc = 0;
		*argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
		if (!*argv)
			return ;
	}
}

int	detect_quote_types(const char *s, int *had_single, int *had_double)
{
	int	i;

	*had_single = 0;
	*had_double = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !*had_double)
			*had_single = 1;
		else if (s[i] == '"' && !*had_single)
			*had_double = 1;
		i++;
	}
	return (0);
}

void	redir_output(t_cmd *cmd, const char *filename, int append)
{
	if (cmd->redir.out_fd > 0)
	{
		close(cmd->redir.out_fd);
		cmd->redir.out_fd = -1;
	}
	if (cmd->redir.append_fd > 0)
	{
		close(cmd->redir.append_fd);
		cmd->redir.append_fd = -1;
	}
	if (append)
		cmd->redir.append_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		cmd->redir.out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((append && cmd->redir.append_fd == -1) || (!append
			&& cmd->redir.out_fd == -1))
	{
		ms_perror((char *)filename);
		cmd->skip_execution = 1;
		return ;
	}
	(void)filename;
}

static void	append_heredoc_node(t_cmd *cmd, t_hdoc *node)
{
	t_hdoc	*t;

	if (!cmd->redir.heredocs)
		cmd->redir.heredocs = node;
	else
	{
		t = cmd->redir.heredocs;
		while (t->next)
			t = t->next;
		t->next = node;
	}
}

void	redir_input_or_heredoc(t_cmd *cmd, const char *filename, int is_heredoc,
		int no_expand_heredoc)
{
	t_hdoc	*node;

	if (is_heredoc)
	{
		node = (t_hdoc *)malloc(sizeof(t_hdoc));
		if (!node)
			return ;
		node->delim = ft_strdup(filename);
		node->no_expand = no_expand_heredoc;
		node->next = NULL;
		append_heredoc_node(cmd, node);
		cmd->redir.heredoc_fd = -2;
	}
	else
	{
		cmd->redir.in_fd = open(filename, O_RDONLY);
		cmd->redir.has_redir_in = 1;
		if (cmd->redir.in_fd == -1)
		{
			ms_perror((char *)filename);
			cmd->skip_execution = 1;
			cmd->is_builtin = 0;
		}
	}
}
