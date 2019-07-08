#include "lemin.h"

static int		is_exclus(int *exclus, int x)
{
	if (exclus[x])
		return (1);
	return (0);
}

static void		enqueue_infix(t_lemin *lemin, t_bt *root, t_queue *q, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		enqueue_infix(lemin, root->right, q, u);
		if (!(lemin->visited)[room->id] && room->edge_flow)
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		enqueue_infix(lemin, root->left, q, u);
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

static  t_icase		*store_result(t_lemin *lemin)
{
	t_icase		*result;
	int			u;

	result = NULL;
	u = lemin->end;
	while (u != lemin->start)
	{
		ic_pushnode(&result, u);
		u = lemin->parent[u];
	}
	ic_pushnode(&result, u);
	return (result);
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
			enqueue_infix(lemin, lemin->tab_bt[u], q, u);
	}
	qt_free(q);
	return (0);
}

static void		pre_enqueue(t_lemin *lemin, t_bt *root, t_queue *q, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		pre_enqueue(lemin, root->right, q, u);
		if (!(lemin->visited)[room->id] && !room->forwd)
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		pre_enqueue(lemin, root->left, q, u);
	}
}

t_icase			*fill_bfs(t_lemin *lemin)
{
	t_queue		*q;
	int			u;
	t_icase		*result;

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
			result = store_result(lemin);
			qt_free(q);
			return (result);
		}
		qt_dequeue(q);
		pre_enqueue(lemin, lemin->tab_bt[u], q, u);
	}
	qt_free(q);
	return (NULL);
}

void		update_forwd(t_bt **tab_bt, int a, int b)
{
	t_bt		*root;
	t_bt		*found;
	t_room		fnd_room;
	t_room		*room;

	root = tab_bt[a];
	fnd_room.id = b;
	found = bt_search_item(root, &fnd_room, &id_cmp);
	room = (t_room*)found->item;
	room->forwd = 1;
	root = tab_bt[b];
	fnd_room.id = a;
	found = bt_search_item(root, &fnd_room, &id_cmp);
	room = (t_room*)found->item;
	room->backwd = 1;
}

void		print_int(char **tab_hash, int *tab, int len)
{
	int		i;

	i = 0;
	while (i < len && tab[i] != -1)
		ft_printf("[%s] - ", tab_hash[tab[i++]]);
	ft_putchar('\n');
}

int			fill_forbackwd(t_lemin *lemin)
{
	t_icase		*result;
	t_icase		*curr;

	/*t_bt	*fnd;
	t_room	*room;
	t_room	fnd_room;*/

	result = NULL;
	while ((result = fill_bfs(lemin)))
	{
		curr = result;
		while (curr && curr->next)
		{
			update_forwd(lemin->tab_bt, curr->n, curr->next->n);
			//	fnd_room.id = curr->next->n;
			//	fnd = bt_search_item(lemin->tab_bt[curr->n], &fnd_room, &id_cmp);
			//	room = (t_room*)fnd->item;
			//	ft_printf("forwd %s-%s: %d\n", lemin->tab_hash[curr->n],
			//		lemin->tab_hash[curr->next->n], room->forwd);
			curr = curr->next;
		}
		//print_ic_hash(result, lemin->tab_hash);		//!!!!!!!!!!!!!
		ic_free(&result);
	//exit(1);
	}
	return (1);
}

int				valid_forbackwd(t_room *room)
{
	if (room->forwd == 1 && room->backwd == 0)
		return (1);
	return (0);
}

static void		enqueue_extended(t_lemin *lemin, t_bt *root, t_queue *q, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		enqueue_extended(lemin, root->right, q, u);
		if (!(lemin->visited)[room->id] && room->ext_edge_flow && valid_forbackwd(room))
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		enqueue_extended(lemin, root->left, q, u);
	}
}

int				extended_bfs(t_lemin *lemin)
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
			enqueue_extended(lemin, lemin->tab_bt[u], q, u);
	}
	qt_free(q);
	return (0);
}

