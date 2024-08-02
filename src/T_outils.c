/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   T_outils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 17:11:46 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/30 13:40:02 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		dup_close(int fd_replace, int fd_erase);
//
t_cmd		*new_node(t_cmd *previous);
t_pipeline	*new_cmd(t_pipeline *previous, char c);

///////////////////////////////////////////////////////////////////////////////]
// duplicate fd_replace onto > fd_erase
// close fd_replace
// dup_close(fd[4], fd[0])
// fd[0] = 4; close(fd[4])
// dup2 and close fd
void	dup_close(int fd_replace, int fd_erase)
{
	if (fd_replace < 0 || fd_erase < 0)
		return ;
	dup2(fd_replace, fd_erase);
	close(fd_replace);
}

///////////////////////////////////////////////////////////////////////////////]
// 		return new node 1 command
t_cmd	*new_node(t_cmd *previous)
{
	t_cmd	*next;

	next = (t_cmd *)mem(0, sizeof(t_cmd));
	if (!next)
		return (NULL);
	if (previous)
		previous->next = next;
	return (next);
}

// 		return new node 1 pipeline
t_pipeline	*new_cmd(t_pipeline *previous, char c)
{
	t_pipeline	*next;

	next = (t_pipeline *)mem(0, sizeof(t_pipeline));
	if (!next)
		return (put(ERRM"error (4)"), NULL);
	if (previous)
	{
		while (previous->next)
			previous = previous->next;
		previous->next = next;
	}
	next->cmd = new_node(NULL);
	next->and_or = c;
	if (!next->cmd)
		return (put(ERRM"error (4)"), NULL);
	return (next);
}
