/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 14:06:05 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	cd_update_env(t_shell *shell, char *oldpwd, char *cwd)
{
	env_set(&shell->envp, "OLDPWD", oldpwd);
	getcwd(cwd, 1024);
	env_set(&shell->envp, "PWD", cwd);
}

static char	*cd_select_target(char **argv, t_shell *shell)
{
	if (!argv[1])
		return (get_env_var("HOME", shell->envp));
	if (ft_strncmp(argv[1], "-", 2) == 0)
		return (get_env_var("OLDPWD", shell->envp));
	return (argv[1]);
}

static void	cd_missing_var_error(char **argv)
{
	if (!argv[1] || argv[1][0] != '-')
		ms_error2("cd", "HOME not set");
	else
		ms_error2("cd", "OLDPWD not set");
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	oldpwd[1024];
	char	cwd[1024];

	if (argv[1] && argv[2])
		return (ms_error2("cd", "too many arguments"), 1);
	getcwd(oldpwd, sizeof(oldpwd));
	path = cd_select_target(argv, shell);
	if (!path)
		return (cd_missing_var_error(argv), 1);
	if (chdir(path) == -1)
		return (ms_perror(path), 1);
	cd_update_env(shell, oldpwd, cwd);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
		ft_putendl_fd(cwd, 1);
	return (0);
}

int	is_delim(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}
