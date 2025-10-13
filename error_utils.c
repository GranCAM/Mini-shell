/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 20:06:27 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	alloc_pipes_array(t_exec_ctx *s)
{
	int	i;

	if (s->n <= 1)
		return (0);
	s->pfd = malloc(sizeof(*s->pfd) * (s->n - 1));
	if (!s->pfd)
		return (ms_perror("malloc"), -1);
	i = 0;
	while (i < s->n - 1)
	{
		s->pfd[i][0] = -1;
		s->pfd[i][1] = -1;
		i++;
	}
	return (0);
}

static int	alloc_pids_array(t_exec_ctx *s)
{
	int	i;

	s->pids = malloc(sizeof(pid_t) * s->n);
	if (!s->pids)
		return (ms_perror("malloc"), -1);
	i = 0;
	while (i < s->n)
	{
		s->pids[i] = -1;
		i++;
	}
	return (0);
}

void	ms_perror(const char *subject)
{
	ft_putstr_fd("minishell: ", 2);
	if (subject)
	{
		ft_putstr_fd((char *)subject, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	ms_error2(const char *subject, const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (subject)
	{
		ft_putstr_fd((char *)subject, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
}

void	ms_syntax_error(const char *tok)
{
	if (ft_strncmp(tok, "newline", 8) == 0)
	{
		ft_putstr_fd("minishell: parse error near `\\n'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd((char *)tok, 2);
		ft_putstr_fd("'\n", 2);
	}
}

int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	alloc_exec_arrays(t_exec_ctx *s)
{
	if (alloc_pipes_array(s) < 0)
		return (-1);
	if (alloc_pids_array(s) < 0)
	{
		free(s->pfd);
		return (-1);
	}
	s->cmd_arr = malloc(sizeof(t_cmd *) * s->n);
	if (!s->cmd_arr)
	{
		free(s->pfd);
		free(s->pids);
		return (ms_perror("malloc"), -1);
	}
	return (0);
}
