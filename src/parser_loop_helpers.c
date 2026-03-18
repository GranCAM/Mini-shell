/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_loop_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:24:47 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:24:50 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	skip_spaces(t_parse_ctx *c)
{
	while (c->input[*c->i] == ' ' || c->input[*c->i] == '\t')
		(*c->i)++;
	if (!c->input[*c->i])
		return (0);
	return (1);
}

void	cleanup_parse_failure(char ***argv, t_cmd **cmds)
{
	if (argv)
		*argv = NULL;
	if (cmds)
		*cmds = NULL;
}

int	process_token_and_append(t_parse_ctx *c, int *argc, char ***argv,
		char **envp)
{
	t_token_ctx	ctx;

	ctx.token = parse_token(c->input, c->i);
	if (!ctx.token || !*ctx.token)
		return (1);
	ctx.tok_dup = ft_strdup(ctx.token);
	if (!ctx.tok_dup)
		return (1);
	ctx.should_expand = token_should_expand(ctx.token);
	ctx.processed = process_token(ctx.tok_dup, envp, ctx.should_expand,
			c->last_status);
	free(ctx.tok_dup);
	if (!ctx.processed)
		return (1);
	if (ctx.processed[0] == '\0' && !*c->current_cmd && *argc == 0)
	{
		free(ctx.processed);
		ctx.processed = ft_strdup("");
		if (!ctx.processed)
			return (1);
	}
	append_or_start_cmd_ctx(c, argc, argv, ctx.processed);
	return (1);
}

int	handle_pipe_or_redir(t_parse_ctx *c, int *argc, char ***argv,
		int *parse_error)
{
	if (c->input[*c->i] == '|')
	{
		if (!pipe_precheck(c, argc, argv))
			return (0);
		if (!pipe_postcheck(c, argv))
			return (0);
		return (2);
	}
	if (c->input[*c->i] == '<' || c->input[*c->i] == '>')
	{
		if (c->input[*c->i] == '<' && c->input[*c->i + 1] == '<'
			&& c->input[*c->i + 2] == '<')
		{
			ms_syntax_error("<");
			*parse_error = 1;
			return (0);
		}
		parse_redir(c, argc, argv, parse_error);
		if (*parse_error)
			return (0);
		return (2);
	}
	return (1);
}

int	loop_iteration(t_parse_ctx *c, int *argc, char ***argv, char **envp)
{
	int	st;
	int	parse_error;

	parse_error = 0;
	if (!skip_spaces(c))
		return (2);
	st = handle_pipe_or_redir(c, argc, argv, &parse_error);
	if (st == 0 || parse_error)
		return (0);
	if (st == 2)
		return (1);
	process_token_and_append(c, argc, argv, envp);
	return (1);
}
