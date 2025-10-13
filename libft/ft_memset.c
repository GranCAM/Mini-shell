/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:28:54 by guigonza          #+#    #+#             */
/*   Updated: 2024/09/19 19:30:29 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*a;
	unsigned char	d;

	i = 0;
	a = (unsigned char *)b;
	d = (unsigned char)c;
	while (i < len)
	{
		a[i] = d;
		i++;
	}
	return (a);
}
