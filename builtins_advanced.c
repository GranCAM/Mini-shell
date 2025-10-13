/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_advanced.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 21:15:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/07 21:34:15 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* EXIT builtin */
static int	is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	print_non_numeric(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (2);
}

int	builtin_exit(char **argv, int last_status)
{
	int	code;

	code = last_status;
	if (argv[1])
	{
		if (argv[2])
		{
			ms_error2("exit", "too many arguments");
			return (1);
		}
		if (!is_numeric(argv[1]))
			return (print_non_numeric(argv[1]));
		code = (unsigned char)ft_atoi(argv[1]);
	}
	return (code);
}

/* EXPORT / UNSET helper functions */
int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_not_valid_export(const char *name)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
