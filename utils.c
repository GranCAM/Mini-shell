/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:26:45 by carbon-m          #+#    #+#             */
/*   Updated: 2025/10/14 17:26:47 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	wait_and_finalize(t_exec_ctx *s, t_shell *shell)
{
	if (s->pids[s->n - 1] > 0)
		waitpid(s->pids[s->n - 1], &s->last_status, 0);
	wait_others(s);
	sigaction(SIGINT, &s->old_int, NULL);
	if (WIFEXITED(s->last_status))
		shell->last_status = WEXITSTATUS(s->last_status);
	else if (WIFSIGNALED(s->last_status))
		shell->last_status = 128 + WTERMSIG(s->last_status);
	else
		shell->last_status = 1;
	return (shell->last_status);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;
	int		safety_counter;

	safety_counter = 0;
	while (cmds && is_valid_cmd_ptr(cmds) && safety_counter < 1000)
	{
		next = cmds->next;
		if (is_valid_cmd_ptr(next) || !next)
			free_cmd_node(cmds);
		cmds = next;
		safety_counter++;
	}
}

void	close_extra_fds(void)
{
	long	max_fd;
	int		fd;

	max_fd = sysconf(_SC_OPEN_MAX);
	if (max_fd < 0)
		max_fd = 1024;
	fd = 3;
	while (fd < max_fd)
	{
		close(fd);
		fd++;
	}
}

int	hd_update_assigned(int *assigned_fd, int rfd, int is_last)
{
	if (is_last)
	{
		if (*assigned_fd != -1)
			close(*assigned_fd);
		*assigned_fd = rfd;
	}
	else
		close(rfd);
	return (1);
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
