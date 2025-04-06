/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 16:50:31 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:09:31 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(int nb)
{
	long	nbr;
	char	c[10];
	int		i;
	int		count;

	nbr = nb;
	i = 0;
	count = 0;
	if (nbr == 0)
		count += ft_putchar('0');
	if (nbr < 0)
	{
		count += ft_putchar('-');
		nbr *= -1;
	}
	while (nbr > 0)
	{
		c[i++] = nbr % 10 + '0';
		nbr /= 10;
	}
	while (i-- > 0)
		count += ft_putchar(c[i]);
	return (count);
}
