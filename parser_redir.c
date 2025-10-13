/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 18:13:43 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	parse_redir(t_parse_ctx *c, int *argc, char ***argv, int *parse_error)
{
	char	redir_type;
	int		append;
	int		is_heredoc;
	int		no_expand_heredoc;
	char	*filename_noq;

	redir_type = classify_redir_and_advance(c, &append, &is_heredoc);
	if (redir_type == '\0')
	{
		ms_syntax_error("<");
		*parse_error = 1;
		return ;
	}
	filename_noq = get_filename_noq(c, is_heredoc, &no_expand_heredoc,
			parse_error);
	if (!filename_noq)
		return ;
	attach_new_cmd_if_needed(c, argc, argv);
	if ((*c->current_cmd)->skip_execution)
	{
		free(filename_noq);
		return ;
	}
	if (redir_type == '>')
		redir_output(*c->current_cmd, filename_noq, append);
	else
		redir_input_or_heredoc(*c->current_cmd, filename_noq, is_heredoc,
			no_expand_heredoc);
	free(filename_noq);
}
