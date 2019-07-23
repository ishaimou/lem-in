/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 06:42:50 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/23 07:11:12 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

static int		correct_syntax(char **s)
{
	char		*str;
	int			eol;
	int			i;

	i = 0;
	str = *s;
	while (str[i] && str[i] != '-')
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	eol = i;
	while (str[++i])
		if (str[i] < 65 && str[i] > 90 )
			return (0);
	(*s)[eol] = '\0';
	return (eol);
}

static void		set_ant_color(t_infos *infos, char *str)
{
	int			eol;
	int			n;

	eol = correct_syntax(&str);
	if (eol > 0)
	{
		n = ft_atoi(str);
		if (n > 0 && n - 1 < infos->ants)
			infos->tab_ants[n - 1].color = the_color(&str[eol + 1], L_BLACK);
	}
}

static void		set_room_color(t_room *rooms, int v, char *str)
{
	int			eol;
	int			n;

	eol = correct_syntax(&str);
	if (eol > 0)
	{
		n = ft_atoi(str);
		if (n >= 0 && n < v)
			rooms[n].color = the_color(&str[eol + 1], L_WHITE);
	}
}

void			take_cmds(t_infos *infos, t_chr *curr)
{
	if (!ft_strncmp(curr->str, "##color branch ", 15))
		infos->color_paths = the_color(&(curr->str[15]), L_WHITE);
	else if (!ft_strncmp(curr->str, "##color ant ", 12))
		set_ant_color(infos, &(curr->str[12]));
	else if (!ft_strncmp(curr->str, "##color room ", 13))
		set_room_color(infos->rooms, infos->v, &(curr->str[13]));
}
