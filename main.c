/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 02:13:56 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/30 05:59:53 by ishaimou         ###   ########.fr       */
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
	lemin->visited = NULL;
	lemin->parent = NULL;
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
	free(tmp->item);
	free(tmp);
	*root = NULL;
}

void	free_lemin(t_lemin *lemin, int error)
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
		free(lemin->tab_bt);
	}
	if (lemin->tab_hash)
		free_tabstr(&(lemin->tab_hash));
	if (lemin->input)
		chr_free(&lemin->input);
	if (lemin->visited)
		free(lemin->visited);
	if (lemin->parent)
		free(lemin->parent);
	if (error)
		ft_error();
	//if (lemin->list_paths)
}

t_room	*create_room(int room_id)
{
	t_room	*room;

	if (!(room = (t_room*)malloc(sizeof(t_room))))
		ft_error();
	room->id = room_id;
	//room->pid = -1;
	room->edge_flow = 1;
	//room->visited = 0;
	room->full = 0;
	return (room);
}

int		gnl_error(t_lemin *lemin, char **line)
{
	int		ret;

	ret = get_next_line(0, line);
	if (ret < 0)
		ft_error();
	chr_pushfront(&lemin->input, *line, 0);
	/*if ((*line)[0] == '#' && (*line)[1] != '#')
	{
		free(*line);
		*line = NULL;
		gnl_error(lemin, line);
	}*/
	return (ret);
}

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
			print_lemin(lemin);
	chr_revprint(lemin->input);
	write(1, "\n", 1);
}

void	bt_enqueue_infix(t_lemin *lemin, t_bt *root, t_queue *q, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		bt_enqueue_infix(lemin, root->left, q, u);
		if (!(lemin->visited)[room->id])
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		bt_enqueue_infix(lemin, root->right, q, u);
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

t_list	*ft_lstnew_sm(void *content, size_t content_size)
{
	t_list	*node;
	void	*cont;

	node = (t_list*)malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	if (content == NULL)
	{
		node->content = NULL;
		node->content_size = 0;
	}
	else
	{
		node->content = content;
		node->content_size = content_size;
	}
	return (node);
}

void	ic_print(t_icase *icase)
{
	while (icase)
	{
		ft_printf("[%d] - ", icase->n);
		icase = icase->next;
	}
	ft_putchar('\n');
}

void		store_path(t_lemin *lemin)
{
	t_icase		*path;
	t_list		*node;
	int			u;

	u = lemin->parent[lemin->end];
	path = NULL;
	while (u != lemin->start)
	{
		ic_pushnode(&path, u);
		u = lemin->parent[u];
	}
	node = ft_lstnew_sm(path, sizeof(path));
	ft_lstadd(&lemin->list_paths, node);
}

void	print_list_paths(t_list	*list_paths)
{
	while (list_paths)
	{
		ic_print((t_icase*)(list_paths->content));
		list_paths = list_paths->next;
	}
}

int			bfs(t_lemin *lemin)
{
	t_queue		*q;
	int			u;
	
	q = qt_new_queue();
	u = lemin->start;
	qt_enqueue(q, &u, sizeof(int));
	(lemin->visited)[u] = 1;
	(lemin->parent)[u] = -1;
	while (!qt_isempty(*q))
	{
		qt_front(*q, &u);
		if (u == lemin->end)
		{
			store_path(lemin);
			qt_free(q);
			return (1);
		}
		qt_dequeue(q);
		bt_enqueue_infix(lemin, lemin->tab_bt[u], q, u);
	}
	qt_free(q);
	return (0);
}

/*
void	algo(t_lemin *lemin)
{
	
}
*/
void	init_tools(t_lemin *lemin)
{
	if (!(lemin->visited = (int*)ft_memalloc(sizeof(int) * lemin->v)))
		free_lemin(lemin, 1);
	if (!(lemin->parent = (int*)malloc(sizeof(int) * lemin->v)))
		free_lemin(lemin, 1);
}

int		main(void)
{
	t_lemin		lemin;

	init_lemin(&lemin);
	parse(&lemin);
	init_tools(&lemin);
	bfs(&lemin);
	print_list_paths(lemin.list_paths);
	//algo(&lemin);
	free_lemin(&lemin, 0);
	return (0);
}	
