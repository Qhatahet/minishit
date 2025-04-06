/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:15:24 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:02:33 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*head;

	head = NULL;
	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->content = content;
	head->next = NULL;
	return (head);
}
/*
int	main(void)
{
	char	*test_content = "qais hatahet";

	t_list *node = ft_lstnew((void *)test_content);
	if (node == NULL)
	{
		printf("Failed to create a new list node.\n");
		return (1);
	}
	printf("Node content: %s\n", (char *)node->content);
	free(node);
	return (0);
}*/
