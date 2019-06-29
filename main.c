/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 02:13:56 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/29 07:57:36 by ishaimou         ###   ########.fr       */
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

int		chr_len(t_chr *chr)
{
	int		len;

	len = 0;
	while (chr)
	{
		len++;
		chr = chr->next;
	}
	return (len);
}

void            free_chr(t_chr **chr)
{
	t_chr   *curr;
	t_chr   *next;

	curr = *chr;
	while (curr)
	{
		next = curr->next;
		if (curr->str)
			free(curr->str);
		free(curr);
		curr = next;
	}
	*chr = NULL;
}

void	chr_pushfront(t_chr **list, char *str, int len)
{
	t_chr	*node;

	if(!(node = (t_chr*)malloc(sizeof(*list))))
		ft_error();
	node->str = ft_strdup(str);
	node->len = len;
	if (!*list)
	{
		*list = node;
		return ;
	}
	node->next = *list;
	*list = node;
}

int		gnl_error(char **line)
{
	int		ret;

	ret = get_next_line(0, line);
	if (ret < 0)
		ft_error();
	return (ret);
}

void	parse_ants(t_lemin *lemin)
{
	char	*line;
	int		i;

	i = 0;
	if (!gnl_error(&line))
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
	if (line[i])
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
	while (gnl_error(&line))
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
		free_chr(list_tmp);
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
		if (!ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (!ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	return (0);
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
		(lemin->tab_hash)[i] = NULL;
	while (list_tmp)
	{
		ind = str_to_ind(lemin->tab_hash, v, list_tmp->str);
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

void	parse_links(t_lemin *lemin, char *bk_line)
{
	int				id[2];
	char			*line;
	int				eol;
	int				v;

	v = lemin->v;
	if (!(lemin->tab_bt = (t_bt**)malloc(sizeof(t_bt*) * (v + 1))))
		ft_error();
	(lemin->tab_bt)[v] = NULL;
	eol = ft_strlen(bk_line);
	id[0] = str_to_ind(lemin->tab_hash, lemin->v, bk_line);
	id[1] = str_to_ind(lemin->tab_hash, lemin->v, &bk_line[eol + 1]);
	add_links(lemin->tab_bt, id[0], id[1]);
	while (gnl_error(&line))
	{
		if (!(eol = is_link(&line)))
		{
			free_lemin(lemin);
			ft_error();
		}
		id[0] = str_to_ind(lemin->tab_hash, lemin->v, line);
		id[1] = str_to_ind(lemin->tab_hash, lemin->v, &line[eol + 1]);
		add_links(lemin->tab_bt, id[0], id[1]);
	}
}

/*
void	print_farm(void)
{
	char	*line;

	while (get_next_line(0, &line) > 0)
	{
		ft_putstr(line);
		free(line);
	}
}
*/

void	parse(t_lemin *lemin)
{
	t_chr	*list_tmp;
	char	*line;

	list_tmp = NULL;
	parse_ants(lemin);
	line = parse_rooms(lemin, &list_tmp);
	//create_tabhash(lemin, list_tmp);
	//free_chr(&list_tmp);
	//parse_links(lemin, line);
	//free(line);
	//print_farm();
}

int		main(void)
{
	t_lemin		lemin;

	init_lemin(&lemin);
	parse(&lemin);
	free_lemin(&lemin);
	return (0);
}	
