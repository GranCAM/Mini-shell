/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 22:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/29 22:05:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <unistd.h>

static int	grow_line(char **buf, size_t *cap)
{
	char	*tmp;
	size_t	new_cap;

	new_cap = (*cap) * 2;
	tmp = (char *)ft_realloc(*buf, *cap, new_cap);
	if (!tmp)
		return (0);
	*buf = tmp;
	*cap = new_cap;
	return (1);
}

char	*ms_read_line_fd(int fd)
{
	char	*cbuf;
	size_t	cap;
	size_t	len;
	char	c;

	cap = 64;
	len = 0;
	cbuf = (char *)malloc(cap);
	if (!cbuf)
		return (NULL);
	if (read(fd, &c, 1) <= 0)
		return (free(cbuf), NULL);
	while (c != '\n')
	{
		if (len + 1 >= cap && !grow_line(&cbuf, &cap))
			return (free(cbuf), NULL);
		cbuf[len++] = c;
		if (read(fd, &c, 1) <= 0)
			break ;
	}
	cbuf[len] = '\0';
	return (cbuf);
}

int	len_for_var(const char *token, int i, char **envp)
{
	int		end;
	int		len_var;
	char	*var;
	char	*value;

	end = read_var_end(token, i);
	len_var = end - i - 1;
	if (len_var == 0)
		return (1);
	var = ft_substr(token, i + 1, len_var);
	value = get_env_var(var, envp);
	free(var);
	if (value)
		return ((int)ft_strlen(value));
	return (0);
}

void	apply_out_redirs(t_cmd *cmd)
{
	if (cmd->redir.out_fd > 0)
	{
		if (dup2(cmd->redir.out_fd, STDOUT_FILENO) == -1)
			_exit(1);
		close(cmd->redir.out_fd);
	}
	if (cmd->redir.append_fd > 0)
	{
		if (dup2(cmd->redir.append_fd, STDOUT_FILENO) == -1)
			_exit(1);
		close(cmd->redir.append_fd);
	}
}

void	apply_in_redirs(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redir.heredoc_fd == -2)
		handle_heredoc(cmd, shell);
	if (cmd->redir.has_redir_in && cmd->redir.in_fd == -1)
		_exit(1);
	if (cmd->redir.in_fd > 0)
	{
		if (dup2(cmd->redir.in_fd, STDIN_FILENO) == -1)
			_exit(1);
		close(cmd->redir.in_fd);
	}
	if (cmd->redir.heredoc_fd >= 0)
	{
		if (dup2(cmd->redir.heredoc_fd, STDIN_FILENO) == -1)
			_exit(1);
		close(cmd->redir.heredoc_fd);
	}
}
