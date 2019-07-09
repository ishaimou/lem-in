/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   best_group.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 05:57:58 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/05 12:23:44 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int		calc_max_shots(t_paths *paths, int n_paths)
{
	int			max_shots;
	int			i;

	i = 0;
	max_shots = 0;
	while (i < n_paths)
	{
		if (paths[i].ants > 0)
			if (max_shots < paths[i].len + paths[i].ants)
				max_shots = paths[i].len + paths[i].ants;
		i++;
	}
	return (max_shots);
}

static int		sum_subpathslen(t_paths *paths, int n_paths, int n_subs)
{
	int			i;
	int			sum;

	sum = 0;
	i = n_paths - 1;
	while (i >= 0 && n_subs-- > 0)
	{
		sum += paths[i].len;
		i--;
	}
	return (sum);
}

static int		calcul_ants_shots(int ants, t_infos *infos, int n_subs)
{
	t_paths	*paths;
	int		n_paths;
	int		nsubs;
	int		phi;
	int		avg;
	int		i;

	n_paths = infos->n_paths;
	paths = infos->paths;
	avg = (ants + sum_subpathslen(paths, n_paths, n_subs)) / n_paths;
	i = n_paths - 1;
	nsubs = n_subs;
	while (i >= 0 && nsubs-- > 0 && ants > 0)
	{
		phi = avg - paths[i].len;
		paths[i].ants = (ants > phi) ? phi : ants;
		ants -= paths[i].ants;
		i--;
	}
	while (ants > 0)
	{
		i = n_paths - 1;
		nsubs = n_subs;
		while (i >= 0 && nsubs-- > 0 && ants > 0)
		{
			paths[i].ants++;
			ants--;
			i--;
		}
	}
	return (calc_max_shots(paths, n_paths));
}

static void		init_paths_ants(t_paths *paths, int n_paths)
{
	int			i;

	i = 0;
	while (i < n_paths)
	{
		paths[i].ants = 0;
		i++;
	}
}

static void		calcul_obeish(int ants, t_infos *infos)
{
	t_paths		*paths;
	int			n_paths;
	int			n_shots;
	int			n_subs;

	n_paths = infos->n_paths;
	paths = infos->paths;
	infos->n_shots = INT_MAX;
	n_subs = 1;
	infos->n_subs = 1;
	while (n_subs <= n_paths)
	{
		init_paths_ants(paths, n_paths);
		n_shots = calcul_ants_shots(ants, infos, n_subs);
		if (n_shots <= infos->n_shots)
		{
			infos->n_shots = n_shots;
			infos->n_subs = n_subs;
		}
		n_subs++;
	}
	init_paths_ants(paths, n_paths);
	calcul_ants_shots(ants, infos, infos->n_subs);
}

static void		fill_grp_infos(t_lemin *lemin, t_list *grp, t_infos *infos)
{
	t_paths	*infos_paths;
	int		i;

	i = 0;
	//sum_pathslen = 0;
	infos->n_paths = ft_list_size(grp);
	infos->paths = (t_paths*)malloc(sizeof(t_paths) * infos->n_paths);
	infos_paths = infos->paths;
	while (grp)
	{
		infos_paths[i].len = ic_size((t_icase*)(grp->content)) - 2;
		//infos_paths[i].ants = 0;
		grp = grp->next;
		i++;
	}
	calcul_obeish(lemin->ants, infos);
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
