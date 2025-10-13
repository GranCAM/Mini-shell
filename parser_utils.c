/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 14:21:13 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdlib.h>

/* forward decl from parser_expand.c */
char	*remove_quotes_preserving_expand(char *token, int *expand_out);

char	*process_token(char *token, char **envp, int expand, int last_status)
{
	char	*no_quotes;
	char	*result;
	int		expand_hint;

	expand_hint = expand;
	no_quotes = remove_quotes_preserving_expand(token, &expand_hint);
	if (!no_quotes)
		return (NULL);
	if (expand_hint)
		result = expand_env_var(no_quotes, envp, last_status);
	else
		result = ft_strdup(no_quotes);
	if (!result)
		result = ft_strdup(no_quotes);
	free(no_quotes);
	return (result);
}

int	token_should_expand(const char *token)
{
	int	in_single;
	int	in_double;
	int	j;

	in_single = 0;
	in_double = 0;
	j = 0;
	while (token[j])
	{
		if (token[j] == '\'' && !in_double)
			in_single = !in_single;
		else if (token[j] == '"' && !in_single)
			in_double = !in_double;
		else if (token[j] == '$' && !in_single)
			return (1);
		j++;
	}
	return (0);
}

void	mark_non_pipeline_skips(t_cmd *cmds)
{
	t_cmd	*cur;

	cur = cmds;
	while (cur)
	{
		if (cur->redir.has_redir_in && cur->redir.in_fd == -1)
			cur->skip_execution = 1;
		cur = cur->next;
	}
}

/* parse_token is now in parser_tokens.c */

/* From parser_build_loop.c */

int	parse_main_loop(t_parse_ctx *c, int *argc, char ***argv, char **envp)
{
	int	st;

	while (c->input[*c->i])
	{
		st = loop_iteration(c, argc, argv, envp);
		if (st == 0)
		{
			cleanup_parse_failure(argv, c->cmds);
			return (0);
		}
		if (!skip_spaces(c))
			break ;
	}
	return (1);
}

int	is_blank_str(const char *s)
{
	int	i;

	i = 0;
	while (s && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s && s[i] == '\0');
}
