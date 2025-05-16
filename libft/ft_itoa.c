/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:26:53 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/14 23:11:28 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	result(char *res, long num, int i, int len)
{
	while (len > i)
	{
		res[len - 1] = num % 10 + '0';
		num /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	long	nbr;
	int		i;
	int		nlen;

	i = 0;
	nlen = numlen(n);
	res = (char *)malloc(nlen + 1);
	if (!res)
		return (0);
	nbr = n;
	if (nbr < 0)
	{
		res[i] = '-';
		nbr *= -1;
		i++;
	}
	result(res, nbr, i, nlen);
	res[nlen] = '\0';
	return (res);
}
/*
#include <stdio.h>

int	main(void)
{
	printf("%s\n", ft_itoa(9));
	return (0);
}*/
