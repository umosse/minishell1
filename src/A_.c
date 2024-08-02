/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:01:46 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/30 17:07:06 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_parsing(t_data *data, char *input)
{
	int		i;
	int		j;
	char	*word;
	char	**tab;
	t_cmd	*ptr;
	
	ptr = data->cmd->cmd;
	tab = NULL;
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == ' ')
		{
			word = malloc(j + 2);
			while (input[j] != ' ')
			{
				word[j] = input[j];
				j++;
			}
			tab = expand_tab(tab, word);
			free_s(word);
			j = 0;
			while (input[i] == ' ')
				i++;
		}
		i++;
		j++;
	}
	ptr->cmd_arg = tab;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]