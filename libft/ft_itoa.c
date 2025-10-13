/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:09:31 by guigonza          #+#    #+#             */
/*   Updated: 2024/09/25 12:12:13 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_numlen(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t			len;
	char			*change;
	unsigned int	num;

	len = ft_numlen(n);
	change = malloc((len + 1) * sizeof(char));
	if (!change)
		return (NULL);
	change[len] = '\0';
	if (n == 0)
		change[0] = '0';
	if (n < 0)
	{
		change[0] = '-';
		num = -n;
	}
	else
		num = n;
	while (num > 0)
	{
		change[--len] = (num % 10) + '0';
		num /= 10;
	}
	return (change);
}
