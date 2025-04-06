/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 09:53:36 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:04:04 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;
	char	c[10];
	int		i;

	nb = n;
	i = 0;
	if (nb == 0)
		write(fd, "0", 1);
	else if (nb < 0)
	{
		nb = -nb;
		write(fd, "-", 1);
	}
	while (nb > 0)
	{
		c[i] = (nb % 10) + '0';
		nb /= 10;
		i++;
	}
	while (--i >= 0)
		write(fd, &c[i], 1);
}
