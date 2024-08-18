/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:01:46 by kalipso           #+#    #+#             */
/*   Updated: 2024/08/18 16:31:49 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_cmd	*ft_nextcmd(t_data *data, char *input, int *i, t_cmd *cmd)
{
	int	j;

	j = 0;
	while (wii(input[j], " \t\n") >= 0)
	{
		(*i)++;
		j++;
	}
}

t_cmd	*ft_nextnode(t_data *data, char *input, int *i, t_cmd *cmd)
{
	
}

int	ft_oneortwo(char *input, int i)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
		{
			if (input[i + 2] == '|' || input[i + 2] == '&')
				return (3);
			else
				return (2);
		}
		else if (input[i + 1] == '&')
			return (3);
	}
	if (input[i] == '&')
	{
		if (input[i + 1] == '&')
		{
			if (input[i + 2] == '|' || input[i + 2] == '&')
				return (3);
			else
				return (2);
		}
		else if (input[i + 1] == '|')
			return (3);
	}
	else
		return (1);
}

int	ft_parsing(t_data *data, char *input)
{
	int		i;
	t_cmd	*ptr;
	int		oneortwo;

	data->cmd = new_cmd(data->cmd, 0);
	if (!data->cmd)
		return (1);
	ptr = data->cmd->cmd;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|' || input[i] == '&')
		{
			oneortwo = ft_oneortwo(input, i);
			if (oneortwo == 1)
				ptr = ft_nextnode(data, &input[i], &i, ptr);
			else if (oneortwo == 2)
				ptr = ft_nextcmd(data, &input[i], &i, ptr);
			else
				return (1);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////]