/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:55:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 19:17:44 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*get_env_var(char *var, char **envp)
{
	int	i;
	int	vlen;

	if (!envp || !var)
		return (NULL);
	vlen = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, vlen) == 0 && envp[i][vlen] == '=')
			return (envp[i] + vlen + 1);
		i++;
	}
	return (NULL);
}

char	**env_dup(char **envp)
{
	int		i;
	int		k;
	char	**dup;

	i = 0;
	while (envp && envp[i])
		i++;
	dup = (char **)malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	k = 0;
	while (k < i)
	{
		dup[k] = ft_strdup(envp[k]);
		k++;
	}
	dup[i] = NULL;
	return (dup);
}

int	env_index_of(char **envp, const char *name)
{
	int	i;
	int	nlen;

	i = 0;
	nlen = ft_strlen(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, nlen) == 0 && envp[i][nlen] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	env_set(char ***envp, const char *name, const char *value)
{
	char	*entry;
	int		idx;

	if (!envp || !name || !value)
		return (1);
	entry = env_make_entry(name, value);
	if (!entry)
		return (1);
	if (!*envp)
		return (env_init_with_entry(envp, entry));
	idx = env_index_of(*envp, name);
	if (idx >= 0)
		return (env_replace_at(*envp, idx, entry));
	return (env_append_entry(envp, entry));
}

int	env_unset(char ***envp, const char *name)
{
	int	idx;

	if (!envp || !*envp || !name)
		return (1);
	idx = env_index_of(*envp, name);
	if (idx < 0)
		return (0);
	return (env_remove_at(envp, idx));
}
