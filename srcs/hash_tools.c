/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 02:13:49 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/05 02:13:51 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	test_hash(char ***tab_hash, char *str, int *ind, int i)
{
	if (!(*tab_hash)[i])
	{
		(*tab_hash)[i] = ft_strdup(str);
		*ind = i;
		return (0);
	}
	if (!ft_strcmp(str, (*tab_hash)[i]))
	{
		free_tabstr(tab_hash);
		ft_error();
	}
	return (1);
}

void		put_in_tabhash(t_lemin *lemin, char *str, int *ind)
{
	int		i;

	i = *ind;
	while (i < lemin->v)
	{
		if (!test_hash(&(lemin->tab_hash), str, ind, i))
			return ;
		i++;
	}
	i = 0;
	while (i < *ind)
	{
		if (!test_hash(&(lemin->tab_hash), str, ind, i))
			return ;
		i++;
	}
}

int		str_to_ind(char **tab_hash, int v, char *str)
{
	int		ind;
	int		i;

	ind = hash_str(str) % v;
	i = ind;
	while (i < v)
	{
		if (!tab_hash[i])
			return (i);
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (!tab_hash[i])
			return (i);
		i++;
	}
	return (-1);
}

int		hash_findid(char **tab_hash, int size, char *str)
{
	int		ind;
	int		i;

	ind = hash_str(str) % size;
	i = ind;
	while (i < size)
	{
		if (tab_hash[i] && !ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (tab_hash[i] && !ft_strcmp(tab_hash[i], str))
			return (i);
		i++;
	}
	return (-1);
}

void	create_tabhash(t_lemin *lemin, t_chr *list_tmp)
{
	int		ind;
	int		v;
	int		i;

	v = chr_len(list_tmp);
	if (!v)
		ft_error();
	lemin->v = v;
	if (!(lemin->tab_hash = (char**)malloc(sizeof(char*) * (v + 1))))
		ft_error();
	i = 0;
	while (i <= v)
		(lemin->tab_hash)[i++] = NULL;
	while (list_tmp)
	{
		ind = str_to_ind(lemin->tab_hash, v, list_tmp->str);
		put_in_tabhash(lemin, list_tmp->str, &ind);
		if (list_tmp->len == 1)
			lemin->start = ind;
		if (list_tmp->len == 2)
			lemin->end = ind;
		list_tmp = list_tmp->next;
	}
}
