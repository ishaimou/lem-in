#include "lemin.h"

void	init_lemin(t_lemin *lemin)
{
	lemin->visited = NULL;
	lemin->parent = NULL;
	lemin->exclus = NULL;
	lemin->input = NULL;
	lemin->tab_bt = NULL;
	lemin->tab_hash = NULL;
	lemin->list_paths = NULL;
	lemin->list_grp = NULL;
	lemin->grp_infos = NULL;
	lemin->best_grp = NULL;
	lemin->ngrp = 0;
}

void	init_tools(t_lemin *lemin)
{
	if (!(lemin->visited = (int*)malloc(sizeof(int) * lemin->v)))
		free_lemin(lemin, 1);
	if (!(lemin->parent = (int*)malloc(sizeof(int) * lemin->v)))
		free_lemin(lemin, 1);
	if (!(lemin->exclus = (int*)malloc(sizeof(int) * lemin->v)))
		free_lemin(lemin, 1);

}

void	init_tab_ants(t_stat_ants *tab_ants, int size)
{
	while (--size >= 0)
	{
		tab_ants[size].finish = 0;
		tab_ants[size].id_path = -1;
		tab_ants[size].pos = NULL;
	}
}
