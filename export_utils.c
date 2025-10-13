/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:25:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 01:27:06 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	count_env(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

static char	**copy_env(char **env)
{
	int		count;
	char	**copy;
	int		i;

	count = count_env(env);
	copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (i-- > 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

static void	sort_ascii(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr && arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strncmp(arr[i], arr[j], 2147483647) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_decl(const char *s)
{
	const char	*eq;

	ft_putstr_fd("declare -x ", 1);
	eq = ft_strchr(s, '=');
	if (!eq)
	{
		ft_putendl_fd((char *)s, 1);
		return ;
	}
	write(1, s, eq - s + 1);
	ft_putchar_fd('"', 1);
	ft_putstr_fd((char *)(eq + 1), 1);
	ft_putchar_fd('"', 1);
	ft_putchar_fd('\n', 1);
}

void	print_export_sorted(char **env)
{
	char	**copy;
	int		i;

	copy = copy_env(env);
	if (!copy)
		return ;
	sort_ascii(copy);
	i = 0;
	while (copy[i])
	{
		print_decl(copy[i]);
		i++;
	}
	i = 0;
	while (copy[i])
	{
		free(copy[i]);
		i++;
	}
	free(copy);
}
