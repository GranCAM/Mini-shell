/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:06:30 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 11:06:30 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	*env_make_entry(const char *name, const char *value)
{
	int		nlen;
	char	*entry;
	int		vlen;

	nlen = ft_strlen(name);
	vlen = ft_strlen(value);
	entry = (char *)malloc(nlen + 1 + vlen + 1);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, name, nlen + 1);
	entry[nlen] = '=';
	ft_strlcpy(entry + nlen + 1, value, vlen + 1);
	return (entry);
}

int	env_init_with_entry(char ***envp, char *entry)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * 2);
	if (!arr)
	{
		free(entry);
		return (1);
	}
	arr[0] = entry;
	arr[1] = NULL;
	*envp = arr;
	return (0);
}

int	env_replace_at(char **envp, int idx, char *entry)
{
	free(envp[idx]);
	envp[idx] = entry;
	return (0);
}

static int	env_count(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

int	env_append_entry(char ***envp, char *entry)
{
	int		count;
	char	**new_env;
	int		i;

	count = env_count(*envp);
	new_env = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(entry);
		return (1);
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

/* env_remove_at moved to env_utils_remove.c */
