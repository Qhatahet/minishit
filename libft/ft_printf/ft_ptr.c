/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:23:57 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:09:22 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_ptr(unsigned long add)
{
	char	*s;
	int		count;

	count = 0;
	s = "0123456789abcdef";
	if (add == 0)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	if (add >= 16)
	{
		count += ft_ptr(add / 16);
		count += ft_putchar(s[add % 16]);
	}
	else
		count += ft_putchar(s[add]);
	return (count);
}
