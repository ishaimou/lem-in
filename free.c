#include "lemin.h"

static void	free_room(t_bt **root)
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

static void	free_tab_bt(t_bt **tab_bt, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		if (tab_bt[i])
			free_room(&(tab_bt[i]));
		i++;
	}
}

static void	free_list_grp(t_list **list_grp)
{
	t_list	*tmp;
	t_list	*curr;

	tmp = *list_grp;
	while (tmp)
	{
		curr = tmp;
		ic_lstdel((t_list**)(&(tmp->content)));
		tmp = tmp->next;
		free(curr);
	}
	*list_grp = NULL;
}

void	free_lemin(t_lemin *lemin, int error)
{
	int		v;

	v = lemin->v;
	if (lemin->tab_bt)
	{
		free_tab_bt(lemin->tab_bt, lemin->v);
		free(lemin->tab_bt);
	}
	if (lemin->tab_hash)
		free_tabstr(&(lemin->tab_hash));
	if (lemin->list_grp)
		free_list_grp(&lemin->list_grp);
	if (lemin->input)
		chr_free(&lemin->input);
	if (lemin->visited)
		free(lemin->visited);
	if (lemin->parent)
		free(lemin->parent);
	if (lemin->exclus)
		free(lemin->exclus);
	if (error)
		ft_error();
}
