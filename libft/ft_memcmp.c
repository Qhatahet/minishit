/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 06:18:19 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:03:03 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*st1;
	unsigned char	*st2;
	size_t			i;

	i = 0;
	st1 = ((unsigned char *)s1);
	st2 = ((unsigned char *)s2);
	while (i < n)
	{
		if (st1[i] != st2[i])
			return (st1[i] - st2[i]);
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	char	s1[] = "abcdefghij";
	char	s2[] = "abcdefgxyz";

	printf("%i\n", ft_memcmp(s1, s2, 9));
	return (0);
}*/
