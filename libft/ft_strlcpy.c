/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:35:18 by guigonza          #+#    #+#             */
/*   Updated: 2024/09/25 18:11:52 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0' && i + 1 < dst_size)
	{
		dst[i] = src[i];
		i++;
	}
	if (dst_size > 0)
		dst[i] = '\0';
	while (src[i] != 0)
		i++;
	return (i);
}
