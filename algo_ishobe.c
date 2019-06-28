#include "lemin.h"

static void		edit_edgeflow(t_bt **tab_bt, int id1, int id2, t_room *room)
{
	t_bt		*root;
	t_bt		*found;
	t_room		*fnd_room;

	room->id = id1;
	root = tab_bt[id2];
	found = bt_search_item(root, room, &id_cmp);
	fnd_room = (t_room*)found->item;
	fnd_room->edge_flow = 0;
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

static void		update_edgeflow(t_lemin *lemin, t_icase *path)
{
	t_room		room;

	if (path->next)
	{
		while (path && path->next)
		{
			edit_edgeflow(lemin->tab_bt, path->n, path->next->n, &room);
			path = path->next;
		}
	}
	else
	{
		limit_edgeflow(lemin, path->n, &room, 1);
		limit_edgeflow(lemin, path->n, &room, 0);
	}
}

static void		update_exclus(t_lemin *lemin, t_icase *path)
{
	while (path)
	{
		lemin->exclus[path->n] = 1;
		path = path->next;
	}
}

int			algo_ishobe(t_lemin *lemin)
{
	t_list	*curr;
	t_icase	*path;
	int		flux;

	flux = lemin->flux;
	reset_tab_int(lemin->exclus, lemin->v, 0);
	while (flux-- && bfs(lemin))
	{
		path = (t_icase*)(lemin->list_paths->content);
		update_edgeflow(lemin, path);
		update_exclus(lemin, path);
	}
	if (!(lemin->list_paths))
		return (0);
	return (1);
}
