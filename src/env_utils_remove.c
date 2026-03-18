/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:27:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 19:28:43 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	count_envp(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**alloc_new_env(int count)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * count);
	return (arr);
}

int	env_remove_at(char ***envp, int idx)
{
	int		count;
	int		i;
	int		j;
	char	**new_env;

	count = count_envp(*envp);
	new_env = alloc_new_env(count);
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != idx)
		{
			new_env[j] = (*envp)[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free((*envp)[idx]);
	free(*envp);
	*envp = new_env;
	return (0);
}

int	handle_dollar(const char *token, int *i, char **envp, size_t status_len)
{
	int	add;

	if (token[*i + 1] == '?')
	{
		*i += 2;
		return ((int)status_len);
	}
	else
	{
		add = len_for_var(token, *i, envp);
		*i = read_var_end(token, *i);
		return (add);
	}
}

void	free_env(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
