/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:30:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/07 20:40:31 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	export_with_equal(char *name, char *eq, t_shell *shell)
{
	*eq = '\0';
	if (!is_valid_identifier(name))
	{
		print_not_valid_export(name);
		*eq = '=';
		return (1);
	}
	env_set(&shell->envp, name, eq + 1);
	*eq = '=';
	return (0);
}

static int	export_variable(char *arg, t_shell *shell)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		return (export_with_equal(arg, eq, shell));
	if (!is_valid_identifier(arg))
	{
		print_not_valid_export(arg);
		return (1);
	}
	env_set(&shell->envp, arg, "");
	return (0);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_export_sorted(shell->envp);
		return (0);
	}
	while (argv[i])
	{
		if (export_variable(argv[i], shell) != 0)
			status = 1;
		i++;
	}
	return (status);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			env_unset(&shell->envp, argv[i]);
		i++;
	}
	return (status);
}
