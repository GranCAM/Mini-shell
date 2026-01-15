/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 18:18:42 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	count_cmd_list(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int	count_argv(char **argv)
{
	int	c;

	c = 0;
	if (!argv)
		return (0);
	while (argv[c])
		c++;
	return (c);
}

int	append_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_argv;
	int		i;

	if (!cmd)
		return (0);
	count = count_argv(cmd->argv);
	new_argv = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = arg;
	new_argv[count + 1] = NULL;
	if (cmd->argv)
		free(cmd->argv);
	cmd->argv = new_argv;
	return (1);
}

static void	start_first_cmd(t_parse_ctx *c, int *argc, char ***argv,
		char *processed)
{
	int	k;

	(*argv)[(*argc)++] = processed;
	(*argv)[*argc] = NULL;
	*c->cmds = add_cmd(*c->cmds, *argv);
	{
		k = 0;
		while (k < *argc)
		{
			free((*argv)[k]);
			k++;
		}
	}
	free(*argv);
	*c->current_cmd = *c->cmds;
	while (*c->current_cmd && (*c->current_cmd)->next)
		*c->current_cmd = (*c->current_cmd)->next;
	*argc = 0;
	*argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
}

void	append_or_start_cmd_ctx(t_parse_ctx *c, int *argc, char ***argv,
		char *processed)
{
	if (!*c->current_cmd && *argc == 0)
		return (start_first_cmd(c, argc, argv, processed));
	if (*c->current_cmd)
		append_arg_to_cmd(*c->current_cmd, processed);
	else
		(*argv)[(*argc)++] = processed;
}
