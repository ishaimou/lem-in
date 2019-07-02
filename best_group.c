#include "lemin.h"

static void		calcul_ants_shots(int ants, t_infos *infos)
{
	t_paths	*paths;
	int		n_paths;
	int		tmp;
	int		max;
	int		i;

	max = 0;
	n_paths = infos->n_paths;
	paths = infos->paths;
	i = n_paths - 1;
	while (--i >= 0 && ants > 0)
	{
		tmp = ants + paths[n_paths - 1].len - 1;
		if (tmp > paths[i].len)
		{
			paths[i].ants = ft_ceil((float)(tmp - paths[i].len) / 2.0);
			ants -= paths[i].ants;
		}
		max = ft_max(max, paths[i].len + paths[i].ants);
	}
	paths[n_paths - 1].ants = ants;
	infos->n_shots = ft_max(max, paths[n_paths - 1].len + ants);
}

static void		fill_grp_infos(t_lemin *lemin, t_list *grp, t_infos *infos)
{
	t_paths	*infos_paths;
	int		i;

	i = 0;
	infos->n_paths = ft_list_size(grp);
	infos->paths = (t_paths*)malloc(sizeof(t_paths) * infos->n_paths);
	infos_paths = infos->paths;
	while (grp)
	{
		infos_paths[i].len = ic_size((t_icase*)(grp->content));
		grp = grp->next;
		i++;
	}
	calcul_ants_shots(lemin->ants, infos);
}

static int		find_min_shots(t_infos *infos, int size)
{
	int		index_min;
	int		i;

	i = 0;
	index_min = 0;
	while (i < size)
	{
		if (infos[index_min].n_shots > infos[i].n_shots)
			index_min = i;
		i++;
	}
	return (index_min);
}

static void		best_choice(t_lemin *lemin)
{
	t_infos	*infos;
	t_list	*grps;
	int		index_min;
	int		i;

	infos = lemin->grp_infos;
	grps = lemin->list_grp;
	index_min = find_min_shots(infos, lemin->ngrp);
	i = 0;
	while (i++ < index_min)
		grps = grps->next;
	lemin->best_grp = (t_list*)(grps->content);
	lemin->best_infos = &(infos[index_min]);
}

void		find_best_grp(t_lemin *lemin)
{
	t_list	*ptr_grp;
	int		ngrp;
	int		i;

	ngrp = lemin->ngrp;
	ft_printf("nbr of groups: %d\n", lemin->ngrp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (!(lemin->grp_infos = (t_infos*)malloc(sizeof(t_infos) * (ngrp))))
		ft_error();
	i = 0;
	ptr_grp = lemin->list_grp;
	while (i < ngrp)
	{
		fill_grp_infos(lemin, (t_list*)(ptr_grp->content), &(lemin->grp_infos[i]));
		ptr_grp = ptr_grp->next;
		i++;
	}
	print_grp_infos(lemin->grp_infos, ngrp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	best_choice(lemin);
}
