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

void	create_tabhash(t_lemin *lemin, t_chr *list_tmp)
{
	int		ind;
	int		v;
	int		i;

	v = chr_len(list_tmp);
	if (!v)
		ft_error();
	lemin->v = v;
	if (!(lemin->tab_hash = (char**)malloc(sizeof(char*) * v)))
		ft_error();
	i = 0;
	while (i < v)
		(lemin->tab_hash)[i++] = NULL;
	while (list_tmp)
	{
		ind = hash_strtoind(lemin->tab_hash, v, list_tmp->str);
		hash_putintab(lemin->tab_hash, v, list_tmp->str, &ind);
		if (list_tmp->len == 1)
			lemin->start = ind;
		if (list_tmp->len == 2)
			lemin->end = ind;
		list_tmp = list_tmp->next;
	}
}
