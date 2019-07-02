#include "lemin.h"

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
	int			i;
	int			j;

	i = -1;
	infos = lemin->best_infos;
	npaths = infos->n_paths;
	while (++i < npaths)
	{
		j = -1;
		while (++j < lemin->ants)
		{
			if (!tab_ants[j].finish && valid_path_id(tab_ants[j].id_path, i))
			{
				if (!tab_ants[j].pos)
				{
					tab_ants[j].pos = begin_path(lemin->best_grp, i);
					tab_ants[j].id_path = i;
					print_l(lemin->tab_hash, j + 1, tab_ants[j].pos->n);
					break ;
				}
				else
				{
					tab_ants[j].pos = tab_ants[j].pos->next;
					if (!tab_ants[j].pos)
					{
						tab_ants[j].finish = 1;
						print_l(lemin->tab_hash, j + 1, lemin->end);
					}
					else
						print_l(lemin->tab_hash, j + 1, tab_ants[j].pos->n);
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
	init_tab_ants(tab_ants, lemin->ants);
	while (i++ < nshots)
	{
		parallel_walk(lemin, tab_ants);
		//ft_putstr("- - - - - - - - - -  - - - - - - \n");		//!!!!!!!!!!!!!!!!!!!!!!!
		//print_tab_ants(tab_ants, lemin->ants);					//!!!!!!!!!!!!!!!!!!!!!!!
		//ft_putstr("- - - - - - - - - -  - - - - - - \n");		//!!!!!!!!!!!!!!!!!!!!!!!
	}
	free(tab_ants);
	tab_ants = NULL;
}
