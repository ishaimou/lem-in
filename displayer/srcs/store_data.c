/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 10:28:54 by ishaimou          #+#    #+#             */
/*   Updated: 2019/07/25 10:58:02 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

void			fill_trace(t_infos *infos)
{
	int			*life;
	int			i;
	int			j;

	i = 0;
	while (i < infos->ants)
	{
		j = 0;
		while (j < infos->shots)
		{
			life = infos->tab_ants[i].tab_life;
			if (life[j] != -1 && life[j + 1] != -1 &&
				life[j] != life[j + 1])
			{
				if (infos->links[life[j]][life[j + 1]] == 2)
					break ;
				infos->links[life[j]][life[j + 1]] = 2;
				infos->links[life[j + 1]][life[j]] = 2;
			}
			j++;
		}
		i++;
	}
}

void			fill_room(t_infos *infos, char *str)
{
	char	**tab;
	int		ind;

	tab = ft_strsplit(str, ' ');
	ind = hash_findid(infos->tab_hash, infos->v, tab[0]);
	infos->rooms[ind].id = ind;
	infos->rooms[ind].coord.x = ft_atoi(tab[1]);
	infos->rooms[ind].coord.y = ft_atoi(tab[2]);
	free_tabstr(&tab);
}

void			add_link(char **tab_hash, int v, int **matrix, char *str)
{
	char		**tab;
	int			a;
	int			b;

	tab = ft_strsplit(str, '-');
	a = hash_findid(tab_hash, v, tab[0]);
	b = hash_findid(tab_hash, v, tab[1]);
	matrix[a][b] = 1;
	matrix[b][a] = 1;
}

void			take_options(int ac, char **av, t_infos *infos)
{
	if (ac == 2)
	{
		if (!ft_strcmp(av[1], "-d"))
			infos->debug = 1;
		else
		{
			free_infos(infos);
			ft_dprintf(2, "Usage: ./test [-d]\n");
			ft_dprintf(2, "  -d : enable debug mode\n");
			exit(EXIT_FAILURE);
		}
	}
}

int				store_data(int ac, char **av, t_infos *infos)
{
	init_infos(infos);
	take_options(ac, av, infos);
	if (!(infos->input = gnl_save_chr(0)))
		return(0);
	chr_print(infos->input);
	ft_putstr("\n\n");
	fill_basic_infos(infos);
	if (!alloc_places(infos))
	{
		free_infos(infos);
		exit(0);
	}
	create_tabhash_2(infos, infos->input);
	fill_adv_infos(infos);
	fill_start_end(infos);
	fill_trace(infos);
	if (infos->debug)
		print_debug(infos);
	return (1);
}
