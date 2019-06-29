/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 23:02:05 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/29 08:33:40 by ishaimou         ###   ########.fr       */
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
}					t_lemin;

typedef struct		s_room
{
	int				id;
	int				pid;
	int				edge_flow;
	int				visited:1;
	int				full:1;
}					t_room;

#endif
