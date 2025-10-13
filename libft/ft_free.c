/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:05:54 by guigonza          #+#    #+#             */
/*   Updated: 2025/09/01 12:05:58 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void	ft_free_by_depth(t_format *format)
{
	void	**current_ptr;
	int		i;

	if (!format || !format->ptr)
		return ;
	while (format->depth > 0)
	{
		current_ptr = (void **)format->ptr;
		i = 0;
		while (current_ptr && current_ptr[i])
		{
			free(current_ptr[i]);
			current_ptr[i] = NULL;
			i++;
		}
		free(current_ptr);
		format->ptr = NULL;
		format->depth--;
	}
}

static void	ft_deep_free(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

static void	ft_shallow_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

/* Habría ue ver cómo poder poner mensajes personalizados con variables
Ej: permission denied y el input del archivo que se quiere abrir/cerrar*/

void	ft_error(const char *custom_msg, int count, int type, ...)
{
	va_list		ap;
	int			i;
	t_format	*format;

	format = NULL;
	if (custom_msg)
		ft_putstr_fd((char *)custom_msg, 2);
	va_start(ap, type);
	i = 0;
	while (i++ < count)
	{
		format = va_arg(ap, t_format *);
		if (format)
		{
			if (type == 0)
				ft_shallow_free(format->ptr1);
			else if (type == 1)
				ft_deep_free(format->ptr);
			else if (type == 2)
				ft_free_by_depth(format);
		}
	}
	va_end(ap);
}
