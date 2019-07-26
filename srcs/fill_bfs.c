/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_bfs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 01:09:07 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/26 01:12:20 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_icase	*store_result(t_lemin *lemin)
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

	tabint_reset(lemin->visited, lemin->v, 0);
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
