/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 21:22:55 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:03:25 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d == s || n == 0)
		return (d);
	if (s < d && d < s + n)
	{
		s += n;
		d += n;
		while (n--)
			*(--d) = *(--s);
	}
	else
	{
		while (n--)
			*d++ = *s++;
	}
	return (dest);
}
/*void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*s;
	char	*d;
	char	*temp;
	size_t	i;

	temp = NULL;
	s = (char *)src;
	d = (char *)dest;
	i = 0;
	while (i < n && s[i])
	{
		temp[i] = s[i];
		i++;
	}
	i = 0;
	while (i < n && d[i])
	{
		d[i] = temp[i];
		i++;
	}
	return (d);
}

int	main(void)
{
	char	s[20] = "hatahet";

	ft_memmove(s + 5, s, 4);
	printf("%s\n", s);
	return (0);
}*/
