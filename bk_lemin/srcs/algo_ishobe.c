#include "lemin.h"
/*
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

static void		update_edgeflow(t_lemin *lemin, t_icase *path)
{
	t_room		room;
	t_icase		*tmp;

	if (ic_size(path) == 2)
		extrem_edgeflow(lemin, &room);
	if (ic_size(path) > 3)
	{
		tmp = path->next;
		while (tmp && tmp->next && tmp->next->next)
		{
			edit_edgeflow(lemin->tab_bt, tmp->n, tmp->next->n, &room);
			tmp = tmp->next;
	exit(1);
		}
	}
	else
	{
		limit_edgeflow(lemin, path->next->n, &room, 1);
		limit_edgeflow(lemin, path->next->n, &room, 0);
	}
}*/

static void		update_exclus(t_lemin *lemin, t_icase *path)
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

t_icase		*ic_copy(t_icase *old)
{
	t_icase	*new;

	new = NULL;
	while (old)
	{
		ic_addnode(&new, old->n);
		old = old->next;
	}
	return (new);
}

void		add_listref(t_lemin *lemin, t_icase *path, size_t size)
{
	t_icase		*pref;
	t_list		*father_node;
	t_list		*son_node;
	t_list		*curr;
	t_list		*tmp;

	pref = ic_copy(path);
	if (!(son_node = (t_list*)malloc(sizeof(t_list))))
			free_lemin(lemin, 1);
	son_node->content = pref;
	son_node->next = NULL;
	curr = lemin->list_ref;
	while (curr)
	{
		if (curr->content_size == size)
		{
			tmp = (t_list*)curr->content;
			ft_lstadd(&tmp, son_node);
			return ;
		}
		curr = curr->next;
	}
	if (!(father_node = (t_list*)malloc(sizeof(t_list))))
			free_lemin(lemin, 1);
	father_node->content_size = size;
	father_node->content = son_node;
	father_node->next = NULL;
	ft_lstadd(&(lemin->list_ref), father_node);
}

int			algo_ishobe(t_lemin *lemin)
{
	t_icase	*path;
	//int		flux;

	//flux = lemin->flux;
	reset_tab_int(lemin->exclus, lemin->v, 0);
	if (bfs(lemin, 1))
	{
		path = (t_icase*)(lemin->list_paths->content);
		ft_putstr("//********************************//\n");	
		print_ic_hash(path, lemin->tab_hash);  //!!!!!!!!!!!!!!
		ft_putstr("//********************************//\n");	
		//update_edgeflow(lemin, path);
		update_exclus(lemin, path);
		add_listref(lemin, path, ic_size(path));
		print_list_ref(lemin->list_ref, lemin->tab_hash); //!!!!!!!!!!!
	}
	else
	{
		ft_putstr("......\n");
		return (0);
	}
	while (bfs(lemin, 0))
	{
		path = (t_icase*)(lemin->list_paths->content);
		ft_putstr("__________________________\n");
		print_ic_hash(path, lemin->tab_hash);  //!!!!!!!!!!!!!!
		ft_putstr("__________________________\n");
		//update_edgeflow(lemin, path);
		update_exclus(lemin, path);
	}
	//if (!(lemin->list_paths))
	//	return (0);
	//print_list_ref(lemin->list_ref, lemin->tab_hash);	
	return (1);
}
