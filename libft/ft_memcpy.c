/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:23:04 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:03:14 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*s;
	char	*d;
	size_t	i;

	s = ((char *)src);
	d = ((char *)dest);
	i = 0;
	if (!(s) && !(d))
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (d);
}
/*
int	main(void)
{
	char	s[20] = "qais hatahet";
	char	d[20];

	printf("%p\n",ft_memcpy(((void *)0), ((void *)0), 3));
	//write (1, d, 8);
	return (0);
}*/
