/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   best_group.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 05:57:58 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/04 06:25:09 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int		ft_paths_maxlen(t_paths *paths, int n_paths)
{
	int			max;
	int			i;

	i = 0;
	max = 0;
	while (i < n_paths)
	{
		if (max < paths[i].len)
			max = paths[i].len;
		i++;
	}
	return (max);
}

static int		calc_max_shots(t_paths *paths, int n_paths)
{
	int			max_shots;
	int			i;

	i = 0;
	max_shots = 0;
	while (i < n_paths)
	{
		if (max_shots < paths[i].len + paths[i].ants)
			max_shots = paths[i].len + paths[i].ants;
		i++;
	}
	return (max_shots);
}

static void		calcul_ants_shots(int ants, t_infos *infos)
{
	t_paths	*paths;
	int		n_paths;
	int		phi;
	int		max;
	int		i;

	n_paths = infos->n_paths;
	paths = infos->paths;
	max = ft_paths_maxlen(paths, n_paths);
	i = n_paths - 1;
	while (i >= 0 && ants > 0)
	{
		phi = max - paths[i].len + 1;
		paths[i].ants = ((ants - phi >= 0) ? phi : ants);
		ants -= paths[i].ants;
		i--;
	}
	while (ants > 0)
	{
		i = n_paths - 1;
		while (i >= 0 && ants > 0)
		{
			paths[i].ants++;
			ants--;
			i--;
		}
	}
	infos->n_shots = calc_max_shots(paths, n_paths);
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
		infos_paths[i].len = ic_size((t_icase*)(grp->content)) - 2;
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
	//ft_printf("nbr of groups: %d\n", lemin->ngrp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
