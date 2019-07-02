/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 23:02:05 by ishaimou          #+#    #+#             */
/*   Updated: 2019/07/02 10:39:47 by ishaimou         ###   ########.fr       */
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

typedef struct		s_paths
{
	int				len;
	int				ants;
}					t_paths;

typedef struct		s_infos
{
	int				n_paths;
	t_paths			*paths;
	int				n_shots;
}					t_infos;

typedef struct		s_room
{
	int				id;
	int				edge_flow;
}					t_room;

typedef struct		s_stat_ants
{
	int				finish;
	int				id_path;
	t_icase			*pos;
}					t_stat_ants;

typedef struct		s_lemin
{
	int				ants;
	int				v;
	int				start;
	int				end;
	int				flux;
	int				*visited;
	int				*parent;
	int				*exclus;
	char			**tab_hash;
	t_bt			**tab_bt;
	t_list			*best_grp;
	t_list			*list_grp;
	t_list			*list_paths;
	t_chr			*input;
	t_infos			*grp_infos;
	t_infos			*best_infos;
	int				ngrp;
}					t_lemin;

void		ft_error(void);
void		init_tools(t_lemin *lemin);
void		init_lemin(t_lemin *lemin);
void		init_tab_ants(t_stat_ants *tab_ants, int size);
void		free_lemin(t_lemin *lemin, int error);
void		create_tabhash(t_lemin *lemin, t_chr *list_tmp);
int			hash_findid(char **tab_hash, int size, char *str);
int			str_to_ind(char **tab_hash, int v, char *str);
void		put_in_tabhash(t_lemin *lemin, char *str, int *ind);
int			algo_ishobe(t_lemin *lemin);
int			bfs(t_lemin *lemin);
t_room		*create_room(int room_id);
void		min_flux(t_lemin *lemin);
int			ft_ceil(float f);
int			id_cmp(void *item1, void *item2);
int			gnl_error(t_lemin *lemin, char **line);
void		find_best_grp(t_lemin *lemin);
void		manage_ants(t_lemin *lemin);
void		print_l(char **tab_hash, int a, int b, int *flag);
void 		print_lemin(t_lemin *lemin);			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tabhash(char **tab);				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tabbt(t_bt **tab_bt);				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void 		print_tree(void *item);					//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void		print_list_paths(t_list	*list_paths);	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void		print_list_grp(t_list *grp);			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void		print_path(t_lemin *lemin);				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void		print_grp_infos(t_infos *infos, int ngrp); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void		print_tab_ants(t_stat_ants *tab, int size); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif
