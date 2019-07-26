/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 02:13:56 by ishaimou          #+#    #+#             */
/*   Updated: 2019/07/26 02:17:14 by obelouch         ###   ########.fr       */
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
	if ((*line)[0] == 'L')
		return (0);
	while ((*line)[i] > 32 && (*line)[i] != '-')
		i++;
	if ((*line)[i] != '-')
		return (0);
	eol = i;
	(*line)[i] = '\0';
	i++;
	while (ft_isprint((*line)[i]))
		i++;
	if ((*line)[i])
		return (0);
	return (eol);
}

int			is_room(char **line)
{
	int		i;

	i = 0;
	if ((*line)[0] == 'L')
		return (0);
	while ((*line)[i] > 32 && (*line)[i] != '-')
		i++;
	if ((*line)[i] != ' ')
		return (0);
	(*line)[i] = '\0';
	i++;
	if ((*line)[i] == '-' || (*line)[i] == '+')
		i++;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i] != ' ')
		return (0);
	i++;
	if ((*line)[i] == '-' || (*line)[i] == '+')
		i++;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i])
		return (0);
	return (1);
}

void		parse_cmds(char *line, int *t, int *limit)
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

static int		type_action(t_chr **list_tmp, char **line, int *limit, int *t)
{
	if ((*line)[0] == '#')
	{
		parse_cmds(*line, t, limit);
		return (2);
	}
	if (is_room(line))
	{
		chr_pushfront(list_tmp, *line, *t);
		return (2);
	}
	if (is_link(line))
	{
		if (limit[0] == 1 && limit[1] == 1)
			return (0);
	}
	return (1);
}

char		*parse_rooms(t_lemin *lemin, t_chr **list_tmp)
{
	char	*line;
	int		limit[2];
	int		ret;
	int		t;

	t = 0;
	limit[0] = 0;
	limit[1] = 0;
	while (gnl_error(lemin, &line))
	{
		ret = type_action(list_tmp, &line, limit, &t);
		if (ret == 0)
			return (line);
		if (ret == 1)
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

void	add_links(t_lemin *lemin, int n_vertex, char **link, int eol)
{
	t_room		*room_a;
	t_room		*room_b;
	int			a;
	int			b;

	a = hash_findid(lemin->tab_hash, n_vertex, *link);
	b = hash_findid(lemin->tab_hash, n_vertex, &(*link)[eol]);
	free(*link);
	if (a == -1 || b == -1)
		free_lemin(lemin, 1);
	room_a = create_room(a);
	room_b = create_room(b);
	bt_insert_item(&(lemin->tab_bt)[a], room_b, &id_cmp);
	bt_insert_item(&(lemin->tab_bt)[b], room_a, &id_cmp);
}

void	parse_links(t_lemin *lemin, char **bk_line)
{
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
	add_links(lemin, lemin->v, bk_line, eol + 1);
	while (gnl_error(lemin, &line))
	{
		if (!(eol = is_link(&line)))
		{
			free(line);
			free_lemin(lemin, 1);
		}
		add_links(lemin, lemin->v, &line, eol + 1);
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
		//	print_tabhash(lemin->tab_hash);			//
	parse_links(lemin, &line);
		//	print_tabbt(lemin->tab_bt);				//
	min_flux(lemin);
		//	print_lemin(lemin);						//
	chr_revprint(lemin->input);
	write(1, "\n", 1);
}

void		algo_general_ishobe(t_lemin *lemin)
{
	t_list		*node;
	t_list		*tmp;
	int			i;

	i = 0;
	while (algo_ishobe(lemin))
	{
		ft_printf("{%d}\n", i++);
		//print_list_paths(lemin->list_paths);  //!!!!!!!!!!!!!!!!!!
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
	while (extended_ishobe(lemin))
	{
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
	print_list_grp(lemin->list_grp, lemin->tab_hash);	//////////////////////////
	ft_putstr("\n.............\n\n");	//////////////////////////
	if (!(lemin->list_grp))
		free_lemin(lemin, 1);
}

void	print_l(char **tab_hash, int a, int b, int *flag)
{
	if (*flag)
		ft_putchar(' ');
	if (!*flag)
		*flag = 1;
	ft_putchar('L');
	ft_putnbr(a);
	ft_putchar('-');
	ft_putstr(tab_hash[b]);
}

int		main(void)
{
	t_lemin		lemin;

	init_lemin(&lemin);
	parse(&lemin);
	init_tools(&lemin);
	fill_forbackwd(&lemin);
	algo_general_ishobe(&lemin);
	find_best_grp(&lemin);
	print_grp_infos(lemin.best_infos, 1); //////////////////////////
	ft_putstr("\n.............\n\n");	   //////////////////////////
	//print_list_grp(lemin.list_grp, lemin.tab_hash);    //!!!!!!!!!!
	ft_putstr("((((((((((((((((((THE BEST))))))))))))))))\n"); //!!!!!!!
	print_list_paths(lemin.best_grp, lemin.tab_hash);  //!!!!!!!!!!
	ft_putstr("((((((((((((((((((((()))))))))))))))))))\n");   //!!!!!!!
	manage_ants(&lemin);
	free_lemin(&lemin, 0);
	return (0);
}	
