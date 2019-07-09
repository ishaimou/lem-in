/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 05:36:02 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/09 16:23:45 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_icase	*after_start_path(t_list *best, int index)
{
	int		i;

	i = 0;
	while (i++ < index)
		best = best->next;
	return (((t_icase*)(best->content))->next);
}

static int	valid_path_id(int id, int pass)
{
	if (id == pass)
		return (1);
	if (id == -1)
		return (1);
	return (0);
}

static void		parallel_walk(t_lemin *lemin, t_stat_ants *tab_ants)
{
	t_infos		*infos;
	int			npaths;
	int			nsubs;
	int			i;
	int			j;
	int			flag;

	flag = 0;
	infos = lemin->best_infos;
	nsubs = infos->n_subs;
	npaths = infos->n_paths;
	i = npaths;
	while (--i >= 0 && nsubs-- > 0)
	{
		j = -1;
		while (++j < lemin->ants)
		{
			if (!tab_ants[j].finish && valid_path_id(tab_ants[j].id_path, i))
			{
				if (!tab_ants[j].pos)
				{
					if (infos->paths[i].ants > 0)
					{
						tab_ants[j].pos = after_start_path(lemin->best_grp, i);
						tab_ants[j].id_path = i;
						print_l(lemin->tab_hash, j + 1, tab_ants[j].pos->n, &flag);
						infos->paths[i].ants--;
					}
					break ;
				}
				else
				{
					tab_ants[j].pos = tab_ants[j].pos->next;
					if (tab_ants[j].pos)
						print_l(lemin->tab_hash, j + 1, tab_ants[j].pos->n, &flag);
					else
						tab_ants[j].finish = 1;
				}
			}
		}
	}
	write (1, "\n", 1);
}

void	manage_ants(t_lemin *lemin)
{
	t_stat_ants	*tab_ants;
	int			nshots;
	int			i;

	i = 0;
	nshots = lemin->best_infos->n_shots;
	if (!(tab_ants = (t_stat_ants*)malloc(sizeof(t_stat_ants) * lemin->ants)))
		ft_error();
	//ft_printf("shots: %d\n", nshots);
	init_tab_ants(tab_ants, lemin->ants);
	while (i++ < nshots)
	{
		parallel_walk(lemin, tab_ants);
		//ft_putstr("- - - - - - - - - -  - - - - - - \n");		//!!!!!!!!!!!!!!!!!!!!!!!
		//print_tab_ants(tab_ants, lemin->ants);					//!!!!!!!!!!!!!!!!!!!!!!!
		//ft_putstr("- - - - - - - - - -  - - - - - - \n");		//!!!!!!!!!!!!!!!!!!!!!!!
	}
	ft_putstr("\n");				//!!!!!!!!!!!!!!!!!!!!!
	ft_putstr("nshots: ");			//!!!!!!!!!!!!!!!!!!!!!
	ft_putnbr(nshots);				//!!!!!!!!!!!!!!!!!!!!!
	ft_putstr("\n");				//!!!!!!!!!!!!!!!!!!!!!
	free(tab_ants);
	tab_ants = NULL;
}
