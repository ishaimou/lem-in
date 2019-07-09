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

static void	free_tab_bt(t_bt ***tab_bt, int size)
{
	t_bt	**tmp;
	int		i;

	if (*tab_bt)
	{
		i = 0;
		tmp = *tab_bt;
		while (i < size)
		{
			if (tmp[i])
				free_room(&(tmp[i]));
			i++;
		}
		free(*tab_bt);
	}
}

static void	free_triple_list(t_list **list)
{
	t_list	*tmp;
	t_list	*curr;

	tmp = *list;
	while (tmp)
	{
		curr = tmp;
		ic_lstdel((t_list**)(&(tmp->content)));
		tmp = tmp->next;
		free(curr);
	}
	*list = NULL;
}

static void	free_infos(t_infos **infos, int size)
{
	t_infos *tmp;
	int		i;

	if (*infos)
	{
		i = 0;
		tmp = *infos;
		while (i < size)
		{
			free(tmp[i].paths);
			i++;
		}
		free(*infos);
		*infos = NULL;
	}
}

void	free_tabhash(char ***tab_hash, int size)
{
	int		i;

	i = 0;
	while (i < size)
	{
		free((*tab_hash)[i]);
		i++;
	}
	free(*tab_hash);
}

void	free_lemin(t_lemin *lemin, int error)
{
	int		v;

	v = lemin->v;
	free_tab_bt(&(lemin->tab_bt), lemin->v);
	if (lemin->tab_hash)
		free_tabhash(&(lemin->tab_hash), lemin->v);
	if (lemin->list_grp)
		free_triple_list(&lemin->list_grp);
	if (lemin->list_ref)
		free_triple_list(&lemin->list_ref);
	if (lemin->input)
		chr_free(&lemin->input);
	if (lemin->visited)
		free(lemin->visited);
	if (lemin->parent)
		free(lemin->parent);
	if (lemin->exclus)
		free(lemin->exclus);
	free_infos(&(lemin->grp_infos), lemin->ngrp);
	if (error)
		ft_error();
}
