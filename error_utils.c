/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/14 14:39:21 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	alloc_pipes_array(t_exec_ctx *s)
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

int	alloc_pids_array(t_exec_ctx *s)
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
