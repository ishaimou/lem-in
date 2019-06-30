/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 23:02:05 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/30 02:11:45 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft.h"

typedef struct		s_lemin
{
	int				ants;
	int				v;
	int				start;
	int				end;
	char			**tab_hash;
	t_bt			**tab_bt;
	t_list			*list_paths;
	t_chr			*input;
}					t_lemin;

typedef struct		s_room
{
	int				id;
	int				pid;
	int				edge_flow;
	//int				visited:1;
	int				full:1;
}					t_room;

void		ft_error(void);
void		create_tabhash(t_lemin *lemin, t_chr *list_tmp);
int			hash_findid(char **tab_hash, int size, char *str);
int			str_to_ind(char **tab_hash, int v, char *str);
void		put_in_tabhash(t_lemin *lemin, char *str, int *ind);
void 		print_lemin(t_lemin *lemin);			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tabhash(char **tab);				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tabbt(t_bt **tab_bt);				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tree(void *item);					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif
