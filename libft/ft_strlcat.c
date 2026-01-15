/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:08:16 by guigonza          #+#    #+#             */
/*   Updated: 2024/09/25 17:47:58 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t	len;
	size_t	l2;
	size_t	i;

	len = 0;
	l2 = 0;
	i = 0;
	while (dst[len] != '\0' && len < dst_size)
	{
		len++;
	}
	while (src[l2] != '\0')
		l2++;
	if (dst_size <= len)
		return (dst_size + l2);
	while ((src[i] != '\0') && (len + i) < (dst_size - 1))
	{
		dst[len + i] = src[i];
		i++;
	}
	dst[len + i] = '\0';
	return (len + l2);
}
