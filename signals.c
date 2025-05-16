/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:32:18 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/14 23:02:14 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status;

void	sig(int signum)
{
	(void)signum;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
	g_exit_status = 128 + SIGINT;
}

void	signal_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
