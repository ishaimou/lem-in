/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/22 05:17:29 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/26 02:03:40 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	ft_error(void)
{
	write(2, "ERROR\n", 6);
	exit(1);
}

int		is_exclus(int *exclus, int x)
{
	if (exclus[x])
		return (1);
	return (0);
}

int		is_ignored(char *str)
{
	if (str[0] == '#')
	{
		if (!ft_strcmp(str, "##start"))
			return (0);
		if (!ft_strcmp(str, "##end"))
			return (0);
		return (1);
	}
	return (0);
}

int		gnl_error(t_lemin *lemin, char **line)
{
	int		ret;

	ret = get_next_line(0, line);
	if (ret < 0)
		ft_error();
	chr_pushfront(&lemin->input, *line, 0);
	while (is_ignored(*line))
	{
		free(*line);
		ret = get_next_line(0, line);
		if (ret < 0)
			ft_error();
		chr_pushfront(&lemin->input, *line, 0);
	}
	return (ret);
}

int		id_cmp(void *item1, void *item2)
{
	t_room	*room1;
	t_room	*room2;

	room1 = (t_room*)item1;
	room2 = (t_room*)item2;
	if (room1->id == room2->id)
		return (0);
	if (room1->id > room2->id)
		return (-1);
	return (1);
}

void	min_flux(t_lemin *lemin)
{
	int		conx_start;
	int		conx_end;

	conx_start = bt_size_count(lemin->tab_bt[lemin->start]);
	conx_end = bt_size_count(lemin->tab_bt[lemin->end]);
	if (conx_start < conx_end)
		lemin->flux = ft_min(lemin->ants, conx_start);
	else
		lemin->flux = ft_min(lemin->ants, conx_end);
}

t_room	*create_room(int room_id)
{
	t_room	*room;

	if (!(room = (t_room*)malloc(sizeof(t_room))))
		ft_error();
	room->id = room_id;
	room->ext_edge_flow = 1;
	room->edge_flow = 1;
	room->forwd = 0;
	room->backwd = 0;
	return (room);
}
