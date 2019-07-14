#include "lemin.h"

static void		edit_edgeflow(t_bt **tab_bt, int id1, int id2, t_room *room)
{
	t_bt		*root;
	t_bt		*found;
	t_room		*fnd_room;

	/*room->id = id1;
	root = tab_bt[id2];
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->edge_flow = 0;*/
	room->id = id2;
	root = tab_bt[id1];
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->edge_flow = 0;
}

static void		limit_edgeflow(t_lemin *lemin, int id, t_room *room, int is_start)
{
	t_bt	*root;
	t_bt	*found;
	t_room		*fnd_room;

	root = (is_start) ? (lemin->tab_bt)[lemin->start] : (lemin->tab_bt)[lemin->end];
	if (bt_size_count(root) == 1)
		return ;
	room->id = id;
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->edge_flow = 0;
}

static void		extrem_edgeflow(t_lemin *lemin, t_room *room)
{
	t_bt	*root;
	t_bt	*found;
	t_room		*fnd_room;

	root = (lemin->tab_bt)[lemin->start];
	room->id = lemin->end;
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->edge_flow = 0;
}

int				max_connect(int *connect, int size)
{
	int			i;
	int			max;

	i = 0;
	max = 0;
	while (i < size)
	{
		if (connect[i] > 2)
			return (1);
		i++;
	}
	return (0);
}

static void		update_edgeflow(t_lemin *lemin, t_icase *path)
{
	t_room		room;
	t_icase		*tmp;
	int			size;
	int			connect[2];
	int			obeish;

	obeish = 0;
	size = ic_size(path);
	if (size == 2)
		extrem_edgeflow(lemin, &room);
	if (size > 3)
	{
		tmp = path->next;
		while (tmp && tmp->next && tmp->next->next)
		{
			connect[0] = bt_size_count(lemin->tab_bt[tmp->n]);
			connect[1] = bt_size_count(lemin->tab_bt[tmp->next->n]);
			if (connect[0] > 2 && connect[1] > 2)
			{
					edit_edgeflow(lemin->tab_bt, tmp->n, tmp->next->n, &room);
					obeish = 1;
			}
			tmp = tmp->next;
		}
		if (!obeish)
		{
			tmp = path->next;
			while (tmp && tmp->next && tmp->next->next)
			{
				edit_edgeflow(lemin->tab_bt, tmp->n, tmp->next->n, &room);
				tmp = tmp->next;
			}
		}
	}
	else
	{
		limit_edgeflow(lemin, path->next->n, &room, 1);
		limit_edgeflow(lemin, path->next->n, &room, 0);
	}
}

void		update_exclus(t_lemin *lemin, t_icase *path)
{
	t_icase		*tmp;

	if (path)
		tmp = path->next;
	else
		tmp = NULL;
	while (tmp && tmp->next)
	{
		lemin->exclus[tmp->n] = 1;
		tmp = tmp->next;
	}
}

int			algo_ishobe(t_lemin *lemin)
{
	t_icase	*path;
	//int		flux;

	//flux = lemin->flux;
	tabint_reset(lemin->exclus, lemin->v, 0);
	if (bfs(lemin))
	{
		path = (t_icase*)(lemin->list_paths->content);
		update_edgeflow(lemin, path);
		update_exclus(lemin, path);
	}
	else
		return (0);
	while (bfs(lemin))
	{
		path = (t_icase*)(lemin->list_paths->content);
		update_exclus(lemin, path);
	}
	if (!(lemin->list_paths))
		return (0);
	return (1);
}

static void		ext_edit_edgeflow(t_bt **tab_bt, int id1, int id2, t_room *room)
{
	t_bt		*root;
	t_bt		*found;
	t_room		*fnd_room;

	room->id = id1;
	root = tab_bt[id2];
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->ext_edge_flow = 0;
	room->id = id2;
	root = tab_bt[id1];
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->ext_edge_flow = 0;
}

static void		ext_limit_edgeflow(t_lemin *lemin, int id, t_room *room, int is_start)
{
	t_bt	*root;
	t_bt	*found;
	t_room		*fnd_room;

	root = (is_start) ? (lemin->tab_bt)[lemin->start] : (lemin->tab_bt)[lemin->end];
	if (bt_size_count(root) == 1)
		return ;
	room->id = id;
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->ext_edge_flow = 0;
}

static void		ext_extrem_edgeflow(t_lemin *lemin, t_room *room)
{
	t_bt	*root;
	t_bt	*found;
	t_room		*fnd_room;

	root = (lemin->tab_bt)[lemin->start];
	room->id = lemin->end;
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->ext_edge_flow = 0;
}

static void		ext_update_edgeflow(t_lemin *lemin, t_icase *path)
{
	t_room		room;
	t_icase		*tmp;

	if (ic_size(path) == 2)
		ext_extrem_edgeflow(lemin, &room);
	if (ic_size(path) > 3)
	{
		tmp = path->next;
		while (tmp && tmp->next && tmp->next->next)
		{
			ext_edit_edgeflow(lemin->tab_bt, tmp->n, tmp->next->n, &room);
			tmp = tmp->next;
		}
	}
	else
	{
		ext_limit_edgeflow(lemin, path->next->n, &room, 1);
		ext_limit_edgeflow(lemin, path->next->n, &room, 0);
	}
}

int			extended_ishobe(t_lemin *lemin)
{
	t_icase	*path;

	tabint_reset(lemin->exclus, lemin->v, 0);
	while (extended_bfs(lemin))
	{
		path = (t_icase*)(lemin->list_paths->content);
		ext_update_edgeflow(lemin, path);
		path = (t_icase*)(lemin->list_paths->content);
		update_exclus(lemin, path);
	}
	if (!(lemin->list_paths))
		return (0);
	return (1);
}
