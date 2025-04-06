/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 21:50:58 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 22:59:12 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_atoi(const char *nptr)
{
	char	*num;
	int		res;
	int		sign;

	sign = 1;
	res = 0;
	num = (char *)nptr;
	while (*num == ' ' || (*num >= 9 && *num <= 13))
		num++;
	if (*num == '-' || *num == '+')
	{
		if (*num == '-')
		{
			sign *= -1;
			num++;
		}
		else
			num++;
	}
	while (*num >= '0' && *num <= '9')
	{
		res = (res * 10) + (*num - '0');
		num++;
	}
	return (res * sign);
}
/*
int main (void)
{
	char	s[20] = "-2147483648";

	printf("%i\n", ft_atoi(s));
	return (0);
}*/
