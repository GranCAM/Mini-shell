/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 21:05:37 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <fcntl.h>

static int	has_unclosed_quotes(const char *s)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

t_cmd		*parse_build(char *input, char **envp, int last_status);

t_cmd	*parse_input(char *input, char **envp, int last_status)
{
	if (has_unclosed_quotes(input))
	{
		ms_syntax_error("newline");
		return (NULL);
	}
	return (parse_build(input, envp, last_status));
}

void	expand_named_var(const char *token, int *i, t_expand_ctx *ctx)
{
	int		end;
	int		len_var;
	char	*var;
	char	*value;
	int		k;

	end = read_var_end(token, *i);
	len_var = end - *i - 1;
	if (len_var == 0)
	{
		ctx->dst[ctx->j++] = '$';
		*i = end;
		return ;
	}
	var = ft_substr(token, *i + 1, len_var);
	value = get_env_var(var, ctx->envp);
	free(var);
	k = 0;
	while (value && value[k])
		ctx->dst[ctx->j++] = value[k++];
	*i = end;
}

void	advance_or_count(const char *token, int *i, int *len)
{
	if (token[*i])
	{
		(*len)++;
		(*i)++;
	}
}
