/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 23:02:05 by ishaimou          #+#    #+#             */
/*   Updated: 2019/06/29 06:14:46 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM-IN_H
# define LEM-IN_H

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
}

typedef struct		s_room
{
	int				id;
	int				pid;
	int				edge_flow;
	int				visited:1;
	int				full:1;
}					t_room;

void				parse()

#endif
