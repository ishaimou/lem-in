/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteme.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 05:42:22 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/08 15:26:15 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void        print_ic_hash(t_icase *path, char **tab_hash)
{
	t_icase        *tmp;

	tmp = path;
	while (tmp)
	{
		ft_printf("[%s] - ", tab_hash[tmp->n]);
		tmp = tmp->next;
	}
	ft_putstr("\n");
}

void print_lemin(t_lemin *lemin)
{
	ft_printf("\n+++++++++++++++ lem-in ++++++++++\n");
	ft_printf(" nbr ants   = %d\n", lemin->ants);
	ft_printf(" nbr vertex = %d\n", lemin->v);
	ft_printf(" start = %d\n", lemin->start);
	ft_printf(" end   = %d\n", lemin->end);
	ft_printf(" flux  = %d\n", lemin->flux);
	ft_printf("++++++++++++++++++++++++++++++++++\n");
}

void print_tabhash(char **tab, int size)
{
	int		i = 0;

	while (i < size)
	{
		ft_printf("tab[%d] = %s\n", i, tab[i]);
		i++;
	}
}

void print_tree(void *item)
{
	t_room	*room;

	room = (t_room*)item;
	ft_printf("[%d]-", room->id);
}

void print_tabbt(t_bt **tab_bt)
{
	int		i;

	i = 0;
	while (tab_bt[i])
	{
		ft_printf("\n------------ TREE %d ------------\n", i);
		bt_apply_infix(tab_bt[i], &print_tree);
		i++;
	}
	ft_printf("\n");
}

void	print_list_grp(t_list *grp, char **tab_hash)
{
	t_list	*tmp;
	int		len;
	int		i;

	i = 1;
	len = 0;
	tmp = grp;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	while (grp)
	{
		ft_putstr(" ------ GROUP ");
		ft_putchar('[');
		ft_putnbr(i);
		ft_putchar(']');
		ft_putstr(" --------{ ");
		ft_putnbr(len);
		ft_putstr(" }--------\n");
		print_list_paths((t_list*)grp->content, tab_hash);
		grp = grp->next;
		i++;
		ft_putchar('\n');
	}
}

void	print_list_paths(t_list	*list_paths, char **tab_hash)
{
	t_icase		*path;

	while (list_paths)
	{
		path = (t_icase*)(list_paths->content);
		while (path)
		{
			ft_printf("[%s] - ", tab_hash[path->n]);
			path = path->next;
		}
		ft_putstr("zbi");
		ft_putstr("\n");
		list_paths = list_paths->next;
	}
}

void		print_path(t_lemin *lemin)
{
	int		u;

	u = lemin->end;
	while (u != lemin->start)
	{
		ft_putendl((lemin->tab_hash)[u]);
		u = lemin->parent[u];
	}
	ft_putendl((lemin->tab_hash)[u]);
	ft_putchar('\n');
	ft_putchar('\n');
}

void		print_grp_infos(t_infos *infos, int ngrp)
{
	int		i;
	int		j;
	int		npaths;
	int		nshots;

	i = -1;
	while (++i < ngrp)
	{
		npaths = infos[i].n_paths;
		nshots = infos[i].n_shots;
		ft_putstr("\n--------------------------\n");
		ft_printf("nbr paths: %d\n\n", npaths);
		ft_printf("max shots: %d\n\n", nshots);
		j = -1;
		while (++j < npaths)
		{
			ft_printf("  len paths 	[%d]: %d\n", j + 1, (infos[i].paths)[j].len);
			ft_printf("  ants paths [%d]: %d\n", j + 1, (infos[i].paths)[j].ants);
		}
		ft_putstr("\n--------------------------\n");
	}
}

void		print_tab_ants(t_stat_ants *tab, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		ft_printf("id: %d\t", tab[i].id_path);
		ft_printf("finish: %d\t", tab[i].finish);
		ic_print(tab[i].pos);
		ft_putstr("-   -    -    -    -   -  \n");
		i++;
	}
}
