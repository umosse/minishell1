/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/30 13:40:00 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	clear_cmd(t_data *data);
void	end(t_data *data, int exit_code);

///////////////////////////////////////////////////////////////////////////////]
// free double chain data->cmd
void	clear_cmd(t_data *data)
{
	t_pipeline	*ptr1;
	t_cmd	*ptr2;
	t_cmd	*ptr_temp;

	if (!data->cmd)
		return ;
	ptr1 = data->cmd;
	while (ptr1)
	{
		ptr2 = ptr1->cmd;
		while (ptr2)
		{
			free_tab(ptr2->cmd_arg);
			free_s(ptr2->in_file);
			free_s(ptr2->out_file);
			if (ptr2->fd_in > 0)
				close(ptr2->fd_in);//			flkg m;fglok;;f;dlkm?
			if (ptr2->fd_out > 0)
				close(ptr2->fd_out);
			ptr_temp = ptr2->next;
			free_s(ptr2);
			ptr2 = ptr_temp;
		}
		ptr1 = ptr1->next;
	}
	data->cmd = free_s(data->cmd);
}

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code)
{
	free_tab(data->history);
	free_tab(data->env);
	clear_cmd(data);
	rl_clear_history();
	if (data->fd_in_original >= 0)
		close(data->fd_in_original);
	exit(exit_code);
}
