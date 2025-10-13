/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 19:43:54 by guigonza          #+#    #+#             */
/*   Updated: 2024/09/28 18:58:08 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_obj;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		content = f(lst -> content);
		new_obj = ft_lstnew(content);
		if (!new_obj)
		{
			ft_lstclear(&new_list, del);
			del(content);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_obj);
		lst = lst ->next;
	}
	return (new_list);
}
