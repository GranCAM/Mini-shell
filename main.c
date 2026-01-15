/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 20:49:39 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	int	status;

	if (argc > 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	status = shell_loop(envp);
	return (status);
}

void	dup_io_for_child(t_exec_ctx *s)
{
	if (s->i > 0)
	{
		if (dup2(s->pfd[s->i - 1][0], STDIN_FILENO) == -1)
			_exit(1);
	}
	if (s->i < s->n - 1)
	{
		if (dup2(s->pfd[s->i][1], STDOUT_FILENO) == -1)
			_exit(1);
	}
	close_all_pipes(s->n, s->pfd);
}

void	close_unused_fds(t_exec_ctx *s)
{
	if (s->n > 1)
	{
		if (s->i > 0)
		{
			if (s->pfd[s->i - 1][0] >= 0)
			{
				close(s->pfd[s->i - 1][0]);
				s->pfd[s->i - 1][0] = -1;
			}
		}
		if (s->i < s->n - 1)
		{
			if (s->pfd[s->i][1] >= 0)
			{
				close(s->pfd[s->i][1]);
				s->pfd[s->i][1] = -1;
			}
		}
	}
	close_redir_fds_parent(s->node);
}

void	close_redir_fds_parent(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->redir.in_fd > 0)
	{
		close(cmd->redir.in_fd);
		cmd->redir.in_fd = -1;
	}
	if (cmd->redir.out_fd > 0)
	{
		close(cmd->redir.out_fd);
		cmd->redir.out_fd = -1;
	}
	if (cmd->redir.append_fd > 0)
	{
		close(cmd->redir.append_fd);
		cmd->redir.append_fd = -1;
	}
	if (cmd->redir.heredoc_fd > 0)
	{
		close(cmd->redir.heredoc_fd);
		cmd->redir.heredoc_fd = -1;
	}
}
