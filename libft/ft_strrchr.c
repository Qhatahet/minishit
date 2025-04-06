/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 07:33:48 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:06:45 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*st;
	char	*str;

	st = NULL;
	str = (char *)s;
	while (*str != '\0')
	{
		if (*str == ((char)c))
			st = str;
		str++;
	}
	if (*str == ((char)c))
		st = str;
	return (st);
}
/*
int	main(void)
{
	char	s[] = "qais hatahet";
	char	*p;

	p = ft_strrchr(s, 'h');
	write (1, p, 4);
	return (0);
}*/
