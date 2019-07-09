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
		if (!(lemin->visited)[room->id])// && room->edge_flow)
		{
			qt_enqueue(q, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
		}
		enqueue_infix(lemin, root->left, q, u);
	}
}

static	void	push_iterativ(t_lemin *lemin, t_bt *root, t_list **stack, int u)
{

}

static void		push_infix(t_lemin *lemin, t_bt *root, t_list **stack, int u)
{
	t_room	*room;

	if (root)
	{
		room = (t_room*)root->item;
		push_infix(lemin, root->right, stack, u);
		if (!(lemin->visited)[room->id])// && room->edge_flow)
		{
			sk_push(stack, &room->id, sizeof(int));
			(lemin->visited)[room->id] = 1;
			(lemin->parent)[room->id] = u;
			return ;
		}
		push_infix(lemin, root->left, stack, u);
	}
}

static t_icase	*store_path(t_lemin *lemin)
{
	t_icase		*path;
	int			u;

	u = lemin->end;
	path = NULL;
	while (u != lemin->start)
	{
		ic_pushnode(&path, u);
		u = lemin->parent[u];
	}
	ic_pushnode(&path, u);
	return (path);
}

void			qt_print_hash(t_queue queue, char **tab_hash)
{
	t_list		*curr;

	if (qt_isempty(queue))
	{
		ft_putstr("Empty Queue!\n");
		return ;
	}
	curr = queue.front;
	while (curr)
	{
		ft_putstr(tab_hash[*((int*)(curr->content))]);
		ft_putstr(" <- ");
		curr = curr->next;
	}
	ft_putstr("NULL");
	ft_putchar('\n');
}

int				is_inref(t_lemin *lemin, t_icase *path)
{
	t_list		*tmp;
	size_t		size;

	size = ic_size(path);
	tmp = lemin->list_ref;
	while (tmp)
	{
		if (tmp->content_size == size)
		{
			tmp = (t_list*)tmp->content;
			while (tmp)
			{
				if (!ic_cmp((t_icase*)tmp->content, path))
					/*{
					  ft_putstr("////////////////\\\\\\\\\\\\\\\\\\\\\\\\\n");
					  print_ic_hash(path, lemin->tab_hash);
					  print_ic_hash((t_icase*)tmp->content, lemin->tab_hash);
					  exit(1);*/
					return (1);
				//}
				tmp = tmp->next;
			}
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}

int				bdfs(t_lemin *lemin, int verif)
{
	t_list		*node;
	t_icase		*path;
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
			path = store_path(lemin);
			if (!verif || !is_inref(lemin, path))
			{
				node = ft_lstnew_sm(path, sizeof(path));
				ft_lstadd(&lemin->list_paths, node);
				qt_free(q);
				return (1);
			}
			else
			{
				ic_free(&path);
				//lemin->visited[lemin->end] = 0;
			}
		}
		qt_dequeue(q);
		if (!is_exclus(lemin->exclus, u))
			enqueue_infix(lemin, lemin->tab_bt[u], q, u);
		//qt_print_hash(*q, lemin->tab_hash);
	}
	qt_free(q);
	return (0);
}

int		bfs(t_lemin	*lemin, int verif)
{
	t_list	*stack;
	t_list		*node;
	t_icase		*path;
	int		u;

	stack = NULL;
	reset_tab_int(lemin->visited, lemin->v, 0);
	u = lemin->start;
	sk_push(&stack, &u, sizeof(int));
	(lemin->visited)[u] = 1;
	(lemin->parent)[u] = -1;
	while (!sk_isempty(stack))
	{
		//ft_lstprint(stack, 0, 1);
		u = *(int*)sk_top(stack);
		if (u == lemin->end)
		{
			path = store_path(lemin);
			if (!verif || !is_inref(lemin, path))
			{
				node = ft_lstnew_sm(path, sizeof(path));
				ft_lstadd(&lemin->list_paths, node);
				sk_free(&stack);
				return (1);
			}
			else
				ic_free(&path);
		}
		if (!is_exclus(lemin->exclus, u))
			if (!push_infix(lemin, lemin->tab_bt[u], &stack, u))
				sk_pop(&stack, NULL);
		ft_lstprint(stack, 0, 1);
	}
	sk_free(&stack);
	return (0);
}
