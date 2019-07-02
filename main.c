/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 02:13:56 by ishaimou          #+#    #+#             */
/*   Updated: 2019/07/01 07:46:58 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	parse_ants(t_lemin *lemin)
{
	char	*line;
	int		i;

	i = 0;
	if (!gnl_error(lemin, &line))
	{
		free(line);
		chr_free(&lemin->input);
		ft_error();
	}
	while (ft_isdigit(line[i]))
		i++;
	if (line[i])
	{
		free(line);
		chr_free(&lemin->input);
		ft_error();
	}
	lemin->ants = ft_atoi(line);
	free(line);
	if (!lemin->ants)
	{
		chr_free(&lemin->input);
		ft_error();
	}
}

int			is_link(char **line)
{
	int		eol;
	int		i;

	i = 0;
	while (ft_isalnum((*line)[i]))
		i++;
	if ((*line)[i] != '-')
		return (0);
	eol = i;
	(*line)[i] = '\0';
	i++;
	while (ft_isalnum((*line)[i]))
		i++;
	if ((*line)[i])
		return (0);
	return (eol);
}

int			is_room(char **line)
{
	int		i;

	i = 0;
	while (ft_isalnum((*line)[i]))
		i++;
	if ((*line)[i] != ' ')
		return (0);
	(*line)[i] = '\0';
	i++;
	while (ft_isalnum((*line)[i]))
		i++;
	if ((*line)[i] != ' ')
		return (0);
	i++;
	while (ft_isalnum((*line)[i]))
		i++;
	if ((*line)[i])
		return (0);
	return (1);
}

void		parse_cmds(char *line, int *t, int *limit)
{
	if (line[1] == '#')
	{
		if (!ft_strcmp(&line[2], "start"))
		{
			limit[0] = !limit[0] ? 1 : -1;
			*t = 1;
		}
		else if (!ft_strcmp(&line[2], "end"))
		{
			limit[1] = !limit[1] ? 1 : -1;
			*t = 2;
		}
	}
}

char		*parse_rooms(t_lemin *lemin, t_chr **list_tmp)
{
	char	*line;
	int		limit[2];
	int		t;

	t = 0;
	limit[0] = 0;
	limit[1] = 0;
	while (gnl_error(lemin, &line))
	{
		if (line[0] == '#')
			parse_cmds(line, &t, limit);
		else if (is_room(&line))
			chr_pushfront(list_tmp, line, t);
		else if (is_link(&line))
		{
			if (limit[0] == 1 && limit[1] == 1)
				return (line);
		}
		else
			break ;
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	if (*list_tmp)
		chr_free(list_tmp);
	free_lemin(lemin, 1);
	return (NULL);
}

void	add_links(t_bt **tab_bt, int a, int b)
{
	t_room	*room_a;
	t_room	*room_b;

	room_a = create_room(a);
	room_b = create_room(b);
	bt_insert_item(&tab_bt[a], room_b, &id_cmp);
	bt_insert_item(&tab_bt[b], room_a, &id_cmp);
}

void	parse_links(t_lemin *lemin, char **bk_line)
{
	int				id[2];
	char			*line;
	int				eol;
	int				v;
	int				i;

	v = lemin->v;
	if (!(lemin->tab_bt = (t_bt**)malloc(sizeof(t_bt*) * (v + 1))))
		ft_error();
	i = 0;
	while (i <= v)
		(lemin->tab_bt)[i++] = NULL;
	eol = ft_strlen(*bk_line);
	id[0] = hash_findid(lemin->tab_hash, lemin->v, *bk_line);
	id[1] = hash_findid(lemin->tab_hash, lemin->v, &(*bk_line)[eol + 1]);
	free(*bk_line);
	if (id[0] == -1 || id[1] == -1)
		free_lemin(lemin, 1);
	add_links(lemin->tab_bt, id[0], id[1]);
	while (gnl_error(lemin, &line))
	{
		if (line[0] == '#')
		{
			free(line);
			continue ;
		}
		if (!(eol = is_link(&line)))
		{
			free(line);
			free_lemin(lemin, 1);
		}
		id[0] = hash_findid(lemin->tab_hash, lemin->v, line);
		id[1] = hash_findid(lemin->tab_hash, lemin->v, &line[eol + 1]);
		if (id[0] == -1 || id[1] == -1)
		{
			free(line);
			free_lemin(lemin, 1);
		}
		add_links(lemin->tab_bt, id[0], id[1]);
		free(line);
	}
	free(line);
}

void	parse(t_lemin *lemin)
{
	t_chr	*list_tmp;
	char	*line;

	list_tmp = NULL;
	parse_ants(lemin);
	line = parse_rooms(lemin, &list_tmp);
	create_tabhash(lemin, list_tmp);
	chr_free(&list_tmp);
			print_tabhash(lemin->tab_hash);			//
	parse_links(lemin, &line);
			print_tabbt(lemin->tab_bt);				//
	min_flux(lemin);
			print_lemin(lemin);
	chr_revprint(lemin->input);
	write(1, "\n", 1);
}

int			ft_ceil(float f)
{
	if ((float)(f - (int)f) > .0)
		return ((int)(f + 1));
	return ((int)f);
}

void		calcul_ants_shots(int ants, t_infos *infos)
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

void		fill_grp_infos(t_lemin *lemin, t_list *grp, t_infos *infos)
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

void		best_choice(t_lemin *lemin)
{
	t_list	*ptr_grp;
	int		ngrp;
	int		i;

	ngrp = lemin->ngrp;
	ft_printf("nbr of groups: %d\n", lemin->ngrp);
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
}

void		algo_general_ishobe(t_lemin *lemin)
{
	t_list		*node;
	t_list		*tmp;

	while (algo_ishobe(lemin))
	{
		//print_list_paths(lemin->list_paths);
		node = ft_lstnew_sm(lemin->list_paths, sizeof(t_list*));
		ft_lstadd(&lemin->list_grp, node);
		(lemin->ngrp)++;
		while (lemin->list_paths)
		{
			tmp = lemin->list_paths;
			lemin->list_paths = lemin->list_paths->next;
			tmp = NULL;
		}
	}
		print_list_grp(lemin->list_grp);	//////////////////////////
		ft_putstr("\n.............\n\n");	//////////////////////////
	if (!(lemin->list_grp))
		free_lemin(lemin, 1);
}

int		main(void)
{
	t_lemin		lemin;

	init_lemin(&lemin);
	parse(&lemin);
	init_tools(&lemin);
	algo_general_ishobe(&lemin);
	best_choice(&lemin);
	//print_list_grp(lemin.list_grp);
	free_lemin(&lemin, 0);
	return (0);
}	
