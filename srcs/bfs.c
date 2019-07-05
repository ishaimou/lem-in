#include "lemin.h"

static int		is_exclus(int *exclus, int x)
{
	if (exclus[x])
		return (1);
	return (0);
}

static void		bt_enqueue_infix(t_lemin *lemin, t_bt *root, t_queue *q, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		bt_enqueue_infix(lemin, root->right, q, u);
		if (!(lemin->visited)[room->id] && room->edge_flow)
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		bt_enqueue_infix(lemin, root->left, q, u);
	}
}

static void		store_path(t_lemin *lemin)
{
	t_icase		*path;
	t_list		*node;
	int			u;

	u = lemin->end;
	path = NULL;
	while (u != lemin->start)
	{
		ic_pushnode(&path, u);
		u = lemin->parent[u];
	}
	ic_pushnode(&path, u);
	node = ft_lstnew_sm(path, sizeof(path));
	ft_lstadd(&lemin->list_paths, node);
}

int				bfs(t_lemin *lemin)
{
	t_queue		*q;
	int			u;

	reset_tab_int(lemin->visited, lemin->v, 0);
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
		if (!is_exclus(lemin->exclus, u))
			bt_enqueue_infix(lemin, lemin->tab_bt[u], q, u);
	}
	qt_free(q);
	return (0);
}
