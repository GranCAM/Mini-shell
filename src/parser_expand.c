/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/07 21:36:11 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*build_expand(const char *token, int len, t_expand_ctx *ctx)
{
	int	i;

	ctx->dst = (char *)malloc(sizeof(char) * (len + 1));
	if (!ctx->dst)
		return (NULL);
	i = 0;
	ctx->j = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1])
		{
			if (token[i + 1] == '?')
			{
				expand_status(ctx);
				i += 2;
			}
			else
				expand_named_var(token, &i, ctx);
		}
		else
			ctx->dst[ctx->j++] = token[i++];
	}
	ctx->dst[ctx->j] = '\0';
	return (ctx->dst);
}

int	calc_len_expand(const char *token, char **envp, size_t status_len)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1])
			len += handle_dollar(token, &i, envp, status_len);
		else
			advance_or_count(token, &i, &len);
	}
	return (len);
}

static int	copy_without_quotes(const char *token, char *dst, int *expand_out)
{
	int				i;
	int				j;
	t_expand_ctx	x;

	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] == '\'')
			copy_single_quoted(token, &i, dst, &j);
		else if (token[i] == '"')
		{
			x.dst = dst;
			x.j = j;
			copy_double_quoted(token, &i, &x, expand_out);
			j = x.j;
		}
		else
			copy_unquoted(token, &i, dst, &j);
	}
	return (j);
}

char	*remove_quotes_preserving_expand(char *token, int *expand_out)
{
	int		len;
	char	*result;
	int		j;

	len = ft_strlen(token);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	j = copy_without_quotes(token, result, expand_out);
	result[j] = '\0';
	return (result);
}

char	*expand_env_var(char *token, char **envp, int last_status)
{
	int				len;
	char			*status_str;
	size_t			status_len;
	char			*result;
	t_expand_ctx	ctx;

	if (!envp)
		return (ft_strdup(token));
	status_str = ft_itoa(last_status);
	if (!status_str)
		status_str = ft_strdup("0");
	status_len = ft_strlen(status_str);
	len = calc_len_expand(token, envp, status_len);
	ctx.envp = envp;
	ctx.status_str = status_str;
	ctx.status_len = status_len;
	result = build_expand(token, len, &ctx);
	free(status_str);
	if (!result)
		return (ft_strdup(token));
	return (result);
}
