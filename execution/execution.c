/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:22:32 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/21 19:20:27 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/minishell.h"

extern int g_exit_status;

int	how_many_pipes(t_token *tokens)
{
	t_token *temp;
	int		i;

	i = 0;
	temp = tokens;
	while(temp)
	{
		temp = temp->next;
		i++;
	}
	return (i - 1);
}

int	redirect_first_arg(t_token *tokens)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
		|| temp->type[i] == APPEND || temp->type[i] == HEREDOC)
		return (1);
	return (0);
}

int	is_there_redirect(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		redirect;

	temp = tokens;
	i = 0;
	redirect = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
			|| temp->type[i] == HEREDOC || temp->type[i] == APPEND)
		{
			i += 2;
			redirect++;
		}
		else
			i++;
	}
	return (redirect);
}

int	is_there_command(t_token *tokens)
{
	t_token	*temp;
	int			i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == COMMAND)
			return (1);
		i++;
	}
	return (0);
}

int	is_there_heredoc(t_token *tokens)
{
	t_token	*temp;
	int			i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

int	count_content(t_token *tokens)
{
	int	i;
	t_token	*temp;

	temp = tokens;
	i = 0;
	while(temp->content[i])
		i++;
	return (i);
}

char	**get_env(t_env *env)
{
	t_env	*temp;
	char	**envp;
	char	*tmp;
	int			i;

	temp = env;
	envp = ft_calloc((env_count(env) + 1), sizeof(char *));
	i = 0;
	while (temp)
	{
		tmp = ft_strjoin(temp->variable, "=");
		envp[i] = ft_strjoin(tmp, temp->content);
		free(tmp);
		temp = temp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char	**rearrange_list_redirect(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		i;
	int		j;

	i = 0;
	temp = tokens;
	lst = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
			|| temp->type[i] == APPEND || temp->type[i] == HEREDOC)
			i += 2;
		else
			break ;
	}
	j = 0;
	if (temp->type[i] != REDIRECTIN && temp->type[i] != REDIRECTOUT
		&& temp->type[i] != APPEND && temp->type[i] != HEREDOC && temp->type[i])
	{
		lst[j] = ft_strdup(temp->content[i]);
		if (!lst)
			return (NULL);
		free(temp->content[i]);
		temp->content[i] = ft_strdup("ta3rees");
		j++;	
	}
	i = 0;
	while (temp->content[i] && i < count_content(tokens))
	{
		if (!ft_strcmp(temp->content[i], "ta3rees"))
			i++;
		else if (temp->content[i])
		{
			lst[j] = ft_strdup(temp->content[i]);
			if (!lst[i])
				return (NULL);
			i++;
			j++;
		}
	}
	lst[j] = NULL;
	i = 0;
	return (lst);
}

char **rearrange_list(t_token *tokens)
{
	char	**list;
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	list = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	while (temp->content[i])
	{
		if (temp->content[i])
    		list[i] = ft_strdup(temp->content[i]);
		else
    		list[i] = NULL;
		if (!list[i])
			return (NULL);
		i++;
	}
	list[i] = NULL;
	i = 0;
	return (list);
}

int	open_infiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->in_file = ft_strdup(lst[(*i)]);
	fd->fd_in[(*j)] = open(fd->in_file, O_RDONLY);
	if (fd->fd_in[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->in_file);
		close(fd->saved_in);
		return (fd->fd_in[(*j)]);
	}
	fd->flag_in = 1;
	(*i)++;
	close(fd->fd_in[(*j)]);
	if (lst[(*i)] && !ft_strcmp(lst[(*i)] , "<"))
	{
		free(fd->in_file);
		fd->in_file = NULL;
	}
	return (1);
}

void	dup_in(t_fds *fd, int *j)
{
	if (fd->flag_in)
	{
		fd->fd_in[(*j)] = open(fd->in_file, O_RDONLY);
		free(fd->in_file);
		dup2(fd->fd_in[(*j)], STDIN_FILENO);
		close(fd->fd_in[(*j)]);
	}
	else if (!fd->flag_in)
		close(fd->saved_in);
}

int	open_redirect_in(t_fds *fd, char **lst)
{
	int	i;
	int	j;

	fd->saved_in = dup(STDIN_FILENO);
	fd->flag_in = 0;
	i = 0;
	j = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], "<"))
		{
			if (open_infiles(fd, lst, &i, &j) < 0)
				return (fd->fd_in[j]);
		}
		else
			i++;
	}
	dup_in(fd, &j);
	return (1);
}

int	open_outfiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->out_file = ft_strdup(lst[(*i)]);
	fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->fd_out[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->out_file);
		close(fd->saved_out);
		return (fd->fd_out[(*j)]);
	}
	close(fd->fd_out[(*j)]);
	fd->flag_out = 1;
	(*i)++;
	if (lst[(*i)] && !ft_strcmp(lst[(*i)] , ">"))
	{
		free(fd->out_file);
		fd->out_file = NULL;
	}
	return (1);
}

int	open_appendfiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->out_file = ft_strdup(lst[(*i)]);
	fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd->fd_out[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->out_file);
		close (fd->saved_out);
		return (fd->fd_out[(*j)]);
	}
	close(fd->fd_out[(*j)]);
	fd->flag_append = 1;
	(*i)++;
	if (lst[(*i)] && !ft_strcmp(lst[(*i)] , ">>"))
	{
		free(fd->out_file);
		fd->out_file = NULL;
	}
	return (1);
}

void	dup_out(t_fds *fd, int *j)
{
	if (fd->flag_out)
	{
		fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		free(fd->out_file);
		dup2(fd->fd_out[(*j)], STDOUT_FILENO);
		close(fd->fd_out[(*j)]);
	}
	else if (fd->flag_append)
	{
		fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		free(fd->out_file);
		dup2(fd->fd_out[(*j)], STDOUT_FILENO);
		close(fd->fd_out[(*j)]);
	}
	else if (!fd->flag_out)
		close (fd->saved_out);
}

int	open_redirect_out(t_fds *fd, char	**lst)
{
	int	i;
	int	j;

	fd->saved_out = dup(STDOUT_FILENO);
	fd->flag_out = 0;
	i = 0;
	j = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], ">"))
		{
			if (open_outfiles(fd, lst, &i, &j) < 0)
				return (fd->fd_out[j]);
		}
		else if (!ft_strcmp(lst[i], ">>"))
		{
			if (open_appendfiles(fd, lst, &i, &j) < 0)
				return (fd->fd_out[j]);
		}
		else
			i++;
	}
	dup_out(fd, &j);
	return (1);
}

int	redirect(char **lst, t_shell *shell, t_fds *fd)
{
	int	i;

	i = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], "<<"))
		{
			open_heredoc(shell, lst, fd);
			break;
		}
		i++;
	}
	i = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], ">") || !ft_strcmp(lst[i], ">>")
			|| !ft_strcmp(lst[i], "<"))
		{
			if (open_redirect_out(fd, lst) != 1)
				return (0);
			if (open_redirect_in(fd, lst) != 1)
				return (0);
			break;
		}
		i++;
	}
	return(1);
}

int	redirections(char **lst, int *i)
{
	if (!ft_strcmp(lst[(*i)], ">") || !ft_strcmp(lst[(*i)], "<")
		|| !ft_strcmp(lst[(*i)], ">>") || !ft_strcmp(lst[(*i)], "<<"))
		return (0);
	return (1);
}

char	**open_files(char **lst, t_token *tokens, t_fds *fd, t_shell *shell)
{
	int			i;
	int			j;
	char		**new_list;

	i = 0;
	j = 0;
	if(!redirect(lst, shell, fd))
		return (NULL);
	new_list = ft_calloc((count_content(tokens) + 1) , sizeof(char *));
	while (lst[i])
	{
		if (!redirections(lst, &i))
			i += 2;
		else if (lst[i])
		{
			new_list[j] = ft_strdup(lst[i]);
			if (!new_list[j])
				return (NULL);
			i++;
			j++;
		}
	}
	ft_free_2d(lst);
	new_list[j] = NULL;
	return (new_list);
}

char	**create_list(t_token *tokens, t_fds *fd, t_shell *shell)
{
	char	**lst;
	char	**redirect_lst;
	
	if (redirect_first_arg(tokens))// <in ls after rearrange_list_redirect ls <in
		lst = rearrange_list_redirect(tokens);
	else
		lst = rearrange_list(tokens);//turn the tokens to 2d array
	if (is_there_redirect(tokens))//if there is redirections in the 2d array open all the fucking files and remove them from the list
	{
		redirect_lst = open_files(lst, tokens, fd, shell);
		if (!redirect_lst)
		{
			if (lst)
				ft_free_2d(lst);
			if (fd->saved_in)
				close (fd->saved_in);
			if (fd->saved_out)
				close (fd->saved_out);
			free(fd);
			return (NULL);
		}
		return (redirect_lst);
	}
	else
		return (lst);
}

void	check_files_in_child(t_fds *fd)
{
	int	fds;

	fds = 0;
	if (fd->flag_heredoc)
	{
		fds = open (fd->temp, O_RDONLY);
		fd->flag_in = 1;
		fd->saved_in = dup(STDIN_FILENO);
		unlink(fd->temp);
		dup2(fds, STDIN_FILENO);
		close(fds);
		free(fd->temp);
	}
	if (fd->flag_out)
	{
		close (fd->fd_out[0]);
		close (fd->saved_out);
	}
	if (fd->flag_in)
	{
		close (fd->fd_in[0]);
		close (fd->saved_in);
	}
}

void	restore_in_out(t_fds *fd)
{
	if (fd->flag_out || fd->flag_append)
	{
		dup2(fd->saved_out, STDOUT_FILENO);
		if (fd->saved_out > 0)
			close (fd->saved_out);
	}
	if (fd->flag_in)
	{
		dup2(fd->saved_in, STDIN_FILENO);
		if (fd->saved_in > 0)
			close (fd->saved_in);
	}
	if (fd->temp)
		free(fd->temp);
	if (fd)
		free(fd);
}

void	exit_cmd_not_found(t_shell *shell, t_token *tokens, t_parser *parser, t_fds *fd)
{
	char	*temp;
	char	*string;

	if (shell->cmd_list[0][0] == '.')
	{
		temp = ft_strjoin("ARSSH: ", shell->cmd_list[0]);
		string = ft_strjoin(temp,": permission denied\n");
		free(temp);
		write(2, string, ft_strlen(string));
		free(string);
		free(fd);
		exit_execution(shell, tokens, parser);
		exit(126);
	}
	temp = ft_strjoin("command not found: ", shell->cmd_list[0]);
	string = ft_strjoin(temp,"\n");
	free(temp);
	write(2, string, ft_strlen(string));
	free(string);
	free(fd);
	exit_execution(shell, tokens, parser);
	exit(127);
}

void	execute_cmd_with_path(t_shell *shell, t_token *tokens,
		t_parser *parser, t_fds *fd)
{
	if (!access(shell->cmd_list[0], X_OK))
				shell->cmd = ft_strdup(shell->cmd_list[0]);
	else
		exit_cmd_not_found(shell, tokens, parser, fd);
}

int	execute_built_in(t_shell *shell, t_token *tokens, t_fds *fd)
{
	if (ft_executor(shell, tokens))
	{
		if(shell->fd_out)
			close(shell->fd_out);
		if (fd->flag_out || fd->flag_append)
		{
			dup2(fd->saved_out, STDOUT_FILENO);
			close (fd->saved_out);
		}
		ft_free_2d(shell->cmd_list);
		// exit_execution(shell,tokens,parser);
		return (1);
	}
	return(0);
}

void	get_exit_status(int id)
{
	int status;

	waitpid(id, &status, 0);
    if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

void	execute_command(t_token *tokens, t_shell *shell, t_parser *parser)
{
	t_fds	*fd;
	int		id;
	int		j;

	fd = ft_calloc(1, sizeof(t_fds));
	fd->temp = NULL;
	if (!fd)
		return ;
	shell->cmd_list = create_list(tokens, fd, shell);
	if (!shell->cmd_list)
		return ;
	if(fd->flag_out || fd->flag_append)
		shell->fd_out = fd->fd_out[0];
	if (execute_built_in(shell, tokens, fd))
		return ;
	id = fork();
	if (id == 0)
	{
		g_exit_status = 0;
		j = 0;
		check_files_in_child(fd);
		get_paths(shell);
		if (!shell->paths)
		{
			shell->paths = NULL;
			exit_cmd_not_found(shell, tokens, parser, fd);
		}
		if (!shell->enviroment)
			shell->enviroment = get_env(shell->env);
		if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.' || shell->cmd_list[0][0] == '/'))
			execute_cmd_with_path(shell, tokens, parser, fd);
		else if (shell->cmd_list[0])
		{
			while (shell->paths && shell->paths[j])
			{
				shell->cmd = ft_strjoin(shell->paths[j], shell->cmd_list[0]);
				if (!shell->cmd)
					exit(EXIT_FAILURE);
				if (!access(shell->cmd, X_OK))
					break;
				free(shell->cmd);
				shell->cmd = NULL;
				j++;
			}
			if (!shell->cmd)
				exit_cmd_not_found(shell, tokens, parser, fd);
		}
		else
		{
			exit_execution(shell, tokens, parser);
			free(fd);
			exit(0);
		}
		execve(shell->cmd, shell->cmd_list, shell->enviroment);
	}
	get_exit_status(id);
	restore_in_out(fd);
	if (shell->cmd_list)
	{
		ft_free_2d(shell->cmd_list);
		shell->cmd_list = NULL;
	}
	if(shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
}

void	execute(t_shell *shell, t_token *tokens, t_parser *parser)
{
	(void)shell;
	if (!tokens->content)
		return ;
	if (how_many_pipes(tokens) > 0)
	{
		printf("there is pipes : %i\n", how_many_pipes(tokens));
		execute_multiple(tokens,shell,parser);
	}
	else
	{
		execute_command(tokens, shell, parser);
	}
}
