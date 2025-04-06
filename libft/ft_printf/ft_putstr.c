/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 15:57:25 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 23:09:36 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		count += 6;
	}
	else
	{
		while (str[i] != '\0')
		{
			count += write(1, &str[i], 1);
			i++;
		}
	}
	return (count);
}
