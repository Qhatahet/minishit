/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:48:16 by qhatahet          #+#    #+#             */
/*   Updated: 2025/01/24 22:38:21 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen_gnl(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup_gnl(char *s)
{
	char	*dup;
	int		i;

	dup = malloc(ft_strlen_gnl(s) + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;	

	if (!s1)
		return (ft_strdup_gnl(s2));
	res = malloc(ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	free(s1);
	return (res);
}

char	*ft_strchr_gnl(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	if (c == '\0' && *s == '\0')
		return (s);
	if (*s)
		return (s);
	else
		return (NULL);
}

void	*ft_calloc_gnl(int nmemb, int size)
{
	char	*p;
	int		i;

	i = 0;
	p = malloc (nmemb * size);
	if (p == NULL)
		return (NULL);
	while (i < (nmemb * size))
	{
		p[i] = '\0';
		i++;
	}
	return (p);
}
