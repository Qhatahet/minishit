/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:23:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/09 14:48:34 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_execution(t_shell *shell, t_token *tokens, t_parser *parser)
{
    if (tokens)
		free_tokenizer(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	free(shell);
	free(parser);
	exit(EXIT_FAILURE);
}
int counter(t_token *tokens)
{
	t_token *tmp;
	int i;
	int count;
	
	count = 0;
	i = 0;
	tmp = tokens;
	while(tmp->content[i])
	{
		if(tmp->type[i] == HEREDOC)
		{
			i++;
			count++;
		}
		else if(tmp->type[i] == REDIRECTIN || tmp->type[i] == REDIRECTOUT
			|| tmp->type[i] == APPEND  || tmp->type[i] == ENDOFFILE
			|| tmp->type[i] == FILENAME)
			i++;
		else if (tmp->type[i] == COMMAND || tmp->type[i] == ARGUMENT
			|| tmp->type[i] == TEXT)
		{
			count++;
			i++;
		}    
	}
	return (count);
}

char	**list_redirect(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		i;
	int		j;

	i = 0;
	temp = tokens;
	lst = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	j = 0;
	while (temp->content[i])
	{
		if(temp->type[i] == HEREDOC || temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT || temp->type[i] == APPEND  || temp->type[i] == ENDOFFILE || temp->type[i] == FILENAME )
            i++;
		else if (temp->type[i] == COMMAND || temp->type[i] == ARGUMENT || temp->type[i] == TEXT )
		{
			lst[j] = ft_strdup(temp->content[i]);
			if (!lst[j])
				printf("hello\n");
                i++;
                j++;
        }
	}
	lst[j] = NULL;
	return (lst);
}

char **copy_command_line(t_token *tokens)
{
	char	**list;
	t_token	*temp;
	int		i;
    int     j;

	temp = tokens;
	i = 0;
    j = 0;
	list = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	while (temp->content[i])
	{
        if(temp->type[i] == HEREDOC || temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT || temp->type[i] == APPEND  || temp->type[i] == ENDOFFILE || temp->type[i] == FILENAME )
            i++;
        else if (temp->type[i] == COMMAND || temp->type[i] == ARGUMENT || temp->type[i] == TEXT )
		{
            list[j] = ft_strdup(temp->content[i]);
            if (!list[j])
                printf("hello\n");
            j++;
            i++;
        }
	}
	list[j] = NULL;
	return (list);
}

char **list(t_token *tokens)
{
    char	**list;
    
    if (redirect_first_arg(tokens))
		list = list_redirect(tokens);
	else
		list = copy_command_line(tokens);
    for(int i = 0;list[i] ; i++)
    {   
        fprintf(stderr,"%d:%s\n",i,list[i]);
    }   
	return (list);
}

int	count_heredoc(t_token *tokens)
{
    t_token *temp = tokens;
    int counter = 0;
    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == HEREDOC)
            counter++;
        i++;
    }
    fprintf(stderr,"%d\n",counter);
    return(counter);
}

void	open_heredocs(t_heredoc *heredoc,t_shell *shell)
{
    int i = 0;
    while(heredoc->exit[i])
    {
        char *text = NULL;
        if (heredoc->exit[i][0] == '\'' || heredoc->exit[i][0] == '\"')
            heredoc->exit[i] = remove_qoutes(heredoc->exit[i],shell);
        int fd = open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
        fprintf(stderr,"1\n");
        while (1)
        {
            text = readline(">");
            if(shell->expand_flag)
                text = expand_heredoc(text, shell);
            if (text && heredoc->exit[i] && !ft_strcmp(text, heredoc->exit[i]))
            {
                free(text);
                text = NULL;
                break;
            }
            write(fd, text, ft_strlen(text));
            write(fd, "\n", 1);
            free(text);
        }
        dup2(fd,STDIN_FILENO);
        close(fd);
        i++; 
    }
}

void    is_there_heredoc(t_token *tokens , t_shell *shell)
{
    t_token *temp = tokens;
    t_heredoc *heredoc = malloc(sizeof(t_heredoc));
    
    heredoc->heredocs = malloc((count_heredoc(tokens) + 1)* sizeof(char *));
    heredoc->exit = malloc((count_heredoc(tokens) + 1)* sizeof(char *));
    int i = 0;
    int index = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == HEREDOC)
        {
            heredoc->heredocs[index] = ft_strdup(temp->content[i]);
            i++;
            heredoc->exit[index] = ft_strdup(temp->content[i]);
            index++;
        }
        i++;
    }
    heredoc->heredocs[index] = NULL;
    heredoc->exit[index] = NULL;
    open_heredocs(heredoc,shell);
}

int is_there_redirectin(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTIN)
            return(1);
        i++;
    }
    return (0);
}

int is_there_redirectout(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTOUT)
            return(1);
        i++;
    }
    return (0);
}

char	*open_heredoc_in_pipes(t_fds *fds, t_token *tokens, t_shell *shell)
{
	char	*file;
	int			i;
	int			j;
	char	*delimiter;
	char	*text;
	t_token	*temp;

	temp = tokens;
	file = ft_strdup("temp");
	delimiter = NULL;
	i = 0;
	j = 0;
	fds->flag_expand = 1;
	while(temp->content[i])
	{
		if (!ft_strcmp(temp->content[i], "<<"))
		{
			i++;
			fds->fd_in[j] = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fds->fd_in[j] < 0)
				return (NULL);
			while (1)
			{
				delimiter = ft_strdup(temp->content[i]);
				if (!delimiter)
					return (NULL);
				if (delimiter[0] == '\'' || delimiter[0] == '\"')
					delimiter = remove_qoutes(delimiter, shell);
				text = readline(">");
				if (shell->expand_flag)
				{
					text = expand_heredoc(text, shell); // dollar expand 
				}
				if (!ft_strcmp(text, delimiter))
				{
					free (text);
					text = NULL;
					free(delimiter);
					delimiter = NULL;
					close(fds->fd_in[j]);
					break ;
				}
				write(fds->fd_in[j], text, ft_strlen(text));
				write(fds->fd_in[j], "\n", 1);
				free(text);
				free(delimiter);
			}
			j++;
			free(delimiter);
		}
		i++;
	}
	fds->flag_heredoc = 1;
	return (file);
}

int	how_many_files(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = tokens;
	while (temp)
	{
		i = 0;
		while (temp->content[i])
		{
			if (temp->type[i] == HEREDOC)
			{
				j++;
				break ;
			}
			i++;
		}
		temp = temp->next;
	}
	return (j);
}

void	heredoc_in_pipes(t_token *tokens, t_shell *shell, t_fds *fds)
{
	t_token	*temp;
	char	**heredoc_files;
	int		i;
	int		j;

	temp = tokens;
	i = 0;
	j = 0;
	heredoc_files = ft_calloc(how_many_files(tokens) + 1, sizeof(char *));
	while (temp)
	{
		i = 0;
		while (temp->type[i])
		{
			if (temp->type[i] == HEREDOC)
			{
				heredoc_files[j] = open_heredoc_in_pipes(fds, tokens, shell);
				j++;
				break ;
			}
			i++;
		}
		// fprintf(stderr, "hello there\n");
		temp = temp->next;
	}
}

char	**list_without_heredoc(t_token *tokens)
{
	t_token	*temp;
	char	**list;
	int		i;
	int		j;

	list = malloc(sizeof(char *) * (count_content(tokens) + 1));
	temp = tokens;
	i = 0;
	j = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
			i += 2;
		else
		{
			list[j] = ft_strdup(temp->content[i]);
			j++;
			i++;
		}
	}
	return (list);
}

void execute_multiple(t_token *tokens, t_shell *shell, t_parser *parser)
{
    char *cmd;
    int j = 0;
    int pipes_count = how_many_pipes(tokens);
    int pids[pipes_count + 1];               // Number of commands = pipes_count + 1
    int pipes[pipes_count][2];               // Number of pipes = pipes_count
    int i = 0;
    int heredoc_flag = 0;
	t_fds	*fds;

	fds = malloc(sizeof(t_fds));
    // Create pipes
    while (i < pipes_count) 
    {
        if (pipe(pipes[i]) == -1) 
        {
            fprintf(stderr, "Error with creating pipe\n");
            return;
        }
        i++;
    }
    i = 0;
	heredoc_in_pipes(tokens, shell, fds);

    // Create child processes for each command
    while (i < pipes_count + 1)
    {
        // is_there_heredoc(tokens,shell);
		if (how_many_pipes(tokens))
		{
			shell->cmd_list = list_without_heredoc(tokens);
		}
        pids[i] = fork();
        if (pids[i] == -1)
        {
            fprintf(stderr, "Error with creating process\n");
            return;
        }
        if (pids[i] == 0) 
        {
			// Child process
			heredoc_flag = 0;
			shell->cmd_list = list(tokens);
			get_paths(shell);
			if (!shell->paths) 
			{
				char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
				char *string = ft_strjoin(temp,"\n");
				free(temp);
				write(2, string, ft_strlen(string));
				free(string);
				exit_execution(shell, tokens, parser);
			}
            if (!shell->enviroment)
                shell->enviroment = get_env(shell->env);
            if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.' || shell->cmd_list[0][0] == '/')) 
            {
                if (!access(shell->cmd_list[0], X_OK))
                    cmd = ft_strdup(shell->cmd_list[0]);
            }
            else if (shell->cmd_list[0])
            {
                j = 0;
                while (shell->paths && shell->paths[j]) 
                {
                    cmd = ft_strjoin(shell->paths[j], shell->cmd_list[0]);
                    if (!cmd)
                        exit(EXIT_FAILURE);
                    if (!access(cmd, X_OK)) 
                        break;
                    free(cmd);
                    cmd = NULL;
                    j++;
                }
                if (shell->paths)
                    ft_free_2d(shell->paths);
                if (!cmd) {
                    char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
                    char *string = ft_strjoin(temp,"\n");
                    free(temp);
                    write(2, string, ft_strlen(string));
                    free(string);
                    exit_execution(shell, tokens, parser);
                    exit(EXIT_FAILURE);
                }
            } 
            // else 
            // {
            //     exit_execution(shell, tokens, parser);
            //     exit(0);
            // }
            
            // Close unused pipe ends in the child process
            j = 0;
            while (j < pipes_count) {
                if (i != j + 1) {
                    close(pipes[j][0]);
                }
                if (i != j) {
                    close(pipes[j][1]);
                }
                j++;
            }
            // Redirect input/output for the current command
            if (i == 0) 
            {
                if(is_there_redirect(tokens))
                {
                    dup2(pipes[i][1],STDOUT_FILENO);
					close (pipes[i][1]);
                    int x = 0;
                    int count_rout = 0;
                    int count_rin = 0;
                    int count_append = 0;
                    while(tokens->content[x])
                    {
                        if (tokens->type[x] == REDIRECTOUT) 
                        {
                            count_rout++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == APPEND)
                        {
                            count_append++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_append == 1)
							{
                                dup2(pipes[i - 1][0],STDIN_FILENO);
								close (pipes[i - 1][0]);
							}
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1 && !is_there_redirectout(tokens))
							{
                                dup2(pipes[i][1], STDOUT_FILENO);
								close(pipes[i][1]);
							}
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
					close(pipes[i][1]);
                }
            }
            else if (i == pipes_count)
            {
                if(is_there_redirect(tokens))
                {
                    int x = 0;
                    int count_rout = 0;
                    int count_rin = 0;
                    int count_append = 0;
                    while(tokens->content[x])
                    {
                        if (tokens->type[x] == REDIRECTOUT) 
                        {
                            count_rout++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1 && !is_there_redirectin(tokens))
							{
                                dup2(pipes[i - 1][0],STDIN_FILENO);
								close (pipes[i - 1][0]);
							}
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == APPEND)
                        {
                            count_append++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_append == 1)
							{
                                dup2(pipes[i - 1][0],STDIN_FILENO);
								close (pipes[i - 1][0]);
							}
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
					close(pipes[i - 1][0]);
                    if (i != pipes_count)
					{
                        dup2(pipes[i][1], STDOUT_FILENO);
						close(pipes[i][1]);
					}
                }
            }
            else
            {
                if(is_there_redirect(tokens))
                {
                    dup2(pipes[i][1],STDOUT_FILENO);
					close (pipes[i][1]);
                    int x = 0;
                    int count_rout = 0;
                    int count_rin = 0;
                    int count_append = 0;
                    while(tokens->content[x])
                    {
                        if (tokens->type[x] == REDIRECTOUT) 
                        {
                            count_rout++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1 && !is_there_redirectin(tokens))
							{
                                dup2(pipes[i - 1][0],STDIN_FILENO);
								close(pipes[i - 1][0]);
							}
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == APPEND)
                        {
                            count_append++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_append == 1)
							{
                                dup2(pipes[i - 1][0],STDIN_FILENO);
								close (pipes[i - 1][0]);
							}
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1 && !is_there_redirectout(tokens))
							{
                                dup2(pipes[i][1], STDOUT_FILENO);
								close (pipes[i][1]);
							}
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
					close (pipes[i - 1][0]);
                    if (i != pipes_count)
					{
                        dup2(pipes[i][1], STDOUT_FILENO);
						close(pipes[i][1]);
					}
                }
            }
            if(is_there_command(tokens))
                execve(cmd, shell->cmd_list, shell->enviroment);
            exit(1);
        }
        tokens = tokens->next;
        free(shell->cmd_list);
        shell->cmd_list = NULL;
        i++;
    }
    // Close all pipe ends in the parent process
    j = 0;
    while (j < pipes_count) {
        close(pipes[j][0]);
        close(pipes[j][1]);
        j++;
    }

    // Wait for all child processes
    i = 0;
    while (i < pipes_count + 1) 
    {
        wait(NULL);
        i++;
    }
    return;
}
