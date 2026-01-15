/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 19:27:35 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	pipe_precheck(t_parse_ctx *c, int *argc, char ***argv)
{
	if ((*argc == 0) && (!*c->current_cmd || !(*c->current_cmd)->argv
			|| !(*c->current_cmd)->argv[0]))
	{
		ms_syntax_error("|");
		return (0);
	}
	*c->current_cmd = NULL;
	*argc = 0;
	if (*argv)
		free(*argv);
	*argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
	if (!*argv)
		return (0);
	return (1);
}

int	pipe_postcheck(t_parse_ctx *c, char ***argv)
{
	(void)argv;
	(*c->i)++;
	while (c->input[*c->i] == ' ' || c->input[*c->i] == '\t')
		(*c->i)++;
	if (c->input[*c->i] == '|')
	{
		ms_syntax_error("|");
		return (0);
	}
	return (1);
}

void	copy_single_quoted(const char *t, int *i, char *dst, int *j)
{
	char	q;

	q = '\'';
	(*i)++;
	while (t[*i] && t[*i] != q)
		dst[(*j)++] = t[(*i)++];
	if (t[*i] == q)
		(*i)++;
}

void	copy_double_quoted(const char *t, int *i, t_expand_ctx *x,
		int *expand_out)
{
	char	q;

	q = '"';
	(*i)++;
	while (t[*i] && t[*i] != q)
	{
		if (t[*i] == '$' && expand_out)
			*expand_out = 1;
		x->dst[x->j++] = t[(*i)++];
	}
	if (t[*i] == q)
		(*i)++;
}

void	copy_unquoted(const char *t, int *i, char *dst, int *j)
{
	dst[(*j)++] = t[(*i)++];
}
