/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:21:58 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:22:02 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static t_cmd	*finalize_after_loop(t_parse_ctx *c, int argc, char **argv)
{
	t_cmd	*cmds;

	cmds = *c->cmds;
	if (argv)
	{
		argv[argc] = NULL;
		if (argc > 0)
		{
			cmds = add_cmd(cmds, argv);
			free_split(argv);
		}
		else
		{
			free(argv);
		}
	}
	if (count_cmd_list(cmds) <= 1)
		mark_non_pipeline_skips(cmds);
	return (cmds);
}

t_cmd	*parse_build(char *input, char **envp, int last_status)
{
	int			tmp[2];
	char		**argv;
	t_parse_ctx	c;
	t_cmd		*cur;
	t_cmd		*head;

	argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
	if (!argv)
		return (NULL);
	tmp[0] = 0;
	tmp[1] = 0;
	cur = NULL;
	head = NULL;
	c.input = input;
	c.i = &tmp[1];
	c.cmds = &head;
	c.current_cmd = &cur;
	c.last_status = last_status;
	if (!parse_main_loop(&c, &tmp[0], &argv, envp))
	{
		return (NULL);
	}
	head = finalize_after_loop(&c, tmp[0], argv);
	return (head);
}

int	read_var_end(const char *token, int i)
{
	int	end;

	end = i + 1;
	while (token[end] && (ft_isalnum(token[end]) || token[end] == '_'))
		end++;
	return (end);
}

void	expand_status(t_expand_ctx *ctx)
{
	int	k;

	k = 0;
	while ((size_t)k < ctx->status_len)
		ctx->dst[ctx->j++] = ctx->status_str[k++];
}
