/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils_parse.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:06 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:25:09 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	consume_spaces_and_check_eol(t_parse_ctx *c, int *parse_error)
{
	while (c->input[*c->i] == ' ' || c->input[*c->i] == '\t')
		(*c->i)++;
	if (!c->input[*c->i])
	{
		ms_syntax_error("newline");
		*parse_error = 1;
		return (1);
	}
	return (0);
}

static char	*parse_filename_token(t_parse_ctx *c, int *parse_error)
{
	char	*tok;

	tok = parse_token(c->input, c->i);
	if (!tok || tok[0] == '\0')
	{
		ms_syntax_error("newline");
		*parse_error = 1;
		return (NULL);
	}
	return (tok);
}

static void	compute_heredoc_expand_flag(const char *fname, int is_heredoc,
		int *no_expand_heredoc)
{
	int	had_single;
	int	had_double;

	(void)detect_quote_types(fname, &had_single, &had_double);
	*no_expand_heredoc = (is_heredoc && (had_single || had_double));
}

char	*get_filename_noq(t_parse_ctx *c, int is_heredoc,
		int *no_expand_heredoc, int *parse_error)
{
	char	*filename;
	char	*filename_noq;
	int		dummy;

	if (consume_spaces_and_check_eol(c, parse_error))
		return (NULL);
	filename = parse_filename_token(c, parse_error);
	if (!filename)
		return (NULL);
	compute_heredoc_expand_flag(filename, is_heredoc, no_expand_heredoc);
	dummy = 0;
	filename_noq = remove_quotes_preserving_expand(filename, &dummy);
	if (!filename_noq || !*filename_noq)
	{
		free(filename_noq);
		ms_syntax_error("newline");
		*parse_error = 1;
		return (NULL);
	}
	return (filename_noq);
}

char	classify_redir_and_advance(t_parse_ctx *c, int *append, int *is_heredoc)
{
	char	redir_type;

	*append = 0;
	*is_heredoc = 0;
	redir_type = c->input[*c->i];
	if (redir_type == '>' && c->input[*c->i + 1] == '>')
	{
		*append = 1;
		(*c->i)++;
	}
	else if (redir_type == '<' && c->input[*c->i + 1] == '<')
	{
		*is_heredoc = 1;
		(*c->i)++;
		if (c->input[*c->i + 1] == '<')
			return ('\0');
	}
	(*c->i)++;
	return (redir_type);
}
