/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 02:13:56 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/29 08:33:39 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	ft_error()
{
	write(2, "ERROR\n", 6);
	exit(1);
}

void	init_lemin(t_lemin *lemin)
{
	lemin->input = NULL;
	lemin->tab_bt = NULL;
	lemin->tab_hash = NULL;
	lemin->list_paths = NULL;
}

void    free_room(t_bt **root)
{
	t_bt *tmp;

	tmp = *root;
	if (!*root)
		return ;
	if (tmp->left)
		free_room(&tmp->left);
	if (tmp->right)
		free_room(&tmp->right);
	free(tmp);
	*root = NULL;
}

void	free_lemin(t_lemin *lemin)
{
	int		v;
	int		i;

	v = lemin->v;
	if (lemin->tab_bt)
	{
		i = -1;
		while (++i < v)
			if (lemin->tab_bt[i])
				free_room(&(lemin->tab_bt[i]));
	}
	if (lemin->tab_hash)
		free_tabstr(&(lemin->tab_hash));
	//if (lemin->list_paths)
}

t_room	*create_room(int room_id)
{
	t_room	*room;

	if (!(room = (t_room*)malloc(sizeof(t_room))))
		ft_error();
	room->id = room_id;
	room->pid = -1;
	room->edge_flow = 1;
	room->visited = 0;
	room->full = 0;
	return (room);
}

int		gnl_error(t_lemin *lemin, char **line)
{
	int		ret;

	ret = get_next_line(0, line);
	if (ret < 0)
		ft_error();
	chr_addnode(&(lemin->input), *line, 0);
	return (ret);
}

void	parse_ants(t_lemin *lemin)
{
	char	*line;
	int		i;

	i = 0;
	if (!gnl_error(lemin, &line))
		ft_error();
	while (ft_isdigit(line[i]))
		i++;
	if (line[i])
	{
		free(line);
		ft_error();
	}
	lemin->ants = ft_atoi(line);
	free(line);
	if (!lemin->ants)
		ft_error();
}

int			is_link(char **line)
{
	int		eol;
	int		i;

	i = 0;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i] != '-')
		return (0);
	eol = i;
	(*line)[i] = '\0';
	i++;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i])
		return (0);
	return (eol);
}

int			is_room(char **line)
{
	int		i;

	i = 0;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i] != ' ')
		return (0);
	(*line)[i] = '\0';
	i++;
	while (ft_isdigit((*line)[i]))
		i++;
	if ((*line)[i] != ' ')
		return (0);
	i++;
	while (ft_isdigit((*line)[i]))
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
		{
			free(line);
			break ;
		}
		free(line);
	}
	if (*list_tmp)
	{
		free_lemin(lemin);
		chr_free(list_tmp);
	}
	ft_error();
	return (NULL);
}

static int	test_hash(char ***tab_hash, char *str, int *ind, int i)
{
	if (!(*tab_hash)[i])
	{
		(*tab_hash)[i] = ft_strdup(str);
		*ind = i;
		return (0);
	}
	if (!ft_strcmp(str, (*tab_hash)[i]))
	{
		free_tabstr(tab_hash);
		ft_error();
	}
	return (1);
}

void		put_in_tabhash(t_lemin *lemin, char *str, int *ind)
{
	int		i;

	i = *ind;
	while (i < lemin->v)
	{
		if (!test_hash(&(lemin->tab_hash), str, ind, i))
			return ;
		i++;
	}
	i = 0;
	while (i < *ind)
	{
		if (!test_hash(&(lemin->tab_hash), str, ind, i))
			return ;
		i++;
	}
}

int		str_to_ind(char **tab_hash, int v, char *str)
{
	int		ind;
	int		i;

	ind = hash_str(str) % v;
	i = ind;
	while (i < v)
	{
		if (!tab_hash[i])
			return (i);
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (!tab_hash[i])
			return (i);
		i++;
	}
	return (-1);
}

int		hash_findid(char **tab_hash, int size, char *str)
{
	int		ind;
	int		i;

	ind = hash_str(str) % size;
	i = ind;
	while (i < size)
	{
		if (tab_hash[i] && !ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (tab_hash[i] && !ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	return (-1);
}

void	create_tabhash(t_lemin *lemin, t_chr *list_tmp)
{
	int		ind;
	int		v;
	int		i;

	v = chr_len(list_tmp);
	if (!v)
		ft_error();
	lemin->v = v;
	if (!(lemin->tab_hash = (char**)malloc(sizeof(char*) * (v + 1))))
		ft_error();
	i = 0;
	while (i <= v)
		(lemin->tab_hash)[i++] = NULL;
	while (list_tmp)
	{
		ind = str_to_ind(lemin->tab_hash, v, list_tmp->str);
			ft_putnbr(ind);
			ft_putchar('\n');
		put_in_tabhash(lemin, list_tmp->str, &ind);
		if (list_tmp->len == 1)
			lemin->start = ind;
		if (list_tmp->len == 2)
			lemin->end = ind;
		list_tmp = list_tmp->next;
	}
}

int		id_cmp(void *item1, void *item2)
{
	t_room	*room1;
	t_room	*room2;

	room1 = (t_room*)item1;
	room2 = (t_room*)item2;
	if (room1->id > room2->id)
		return (-1);
	return (1);
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
	{
		free_lemin(lemin);
		ft_error();
	}
	add_links(lemin->tab_bt, id[0], id[1]);
	while (gnl_error(lemin, &line))
	{
		if (!(eol = is_link(&line)))
		{
			free_lemin(lemin);
			ft_error();
		}
		id[0] = hash_findid(lemin->tab_hash, lemin->v, line);
		id[1] = hash_findid(lemin->tab_hash, lemin->v, &line[eol + 1]);
		if (id[0] == -1 || id[1] == -1)
		{
			free_lemin(lemin);
			ft_error();
		}
		add_links(lemin->tab_bt, id[0], id[1]);
	}
}


   void	print_farm(void)
   {
   char	buff[100];

   while (read(0, buff, 100) > 0)
   		ft_putstr(buff);
	}

void print_tabhash(char **tab)
{
	int		i = 0;

	while (tab[i])
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

void print_lemin(t_lemin *lemin)
{
	ft_printf("\n+++++++++++++++ lem-in ++++++++++\n");
	ft_printf(" nbr ants   = %d\n", lemin->ants);
	ft_printf(" nbr vertex = %d\n", lemin->v);
	ft_printf(" start = %d\n", lemin->start);
	ft_printf(" end   = %d\n", lemin->end);
	ft_printf("++++++++++++++++++++++++++++++++++\n");
}

void	chr_printfree(t_chr **list)
{
	t_chr	*tmp;

	while (*list)
	{
		ft_putstr((*list)->str);
		ft_putchar('\n');
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
	*list = NULL;
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
			print_lemin(lemin);
	chr_printfree(&(lemin->input));
}

int		main(void)
{
	t_lemin		lemin;

	init_lemin(&lemin);
	parse(&lemin);
	free_lemin(&lemin);
	return (0);
}	
