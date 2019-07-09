/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 01:41:34 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/05 12:10:39 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

void	chr_print(t_chr *list)
{
	while (list)
	{
		ft_putendl(list->str);
		list = list->next;
	}
}

t_chr			*gnl_save_chr(int fd)
{
	t_chr		*input;
	char		*line;
	int			ret;

	input = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
		chr_addnode(&input, line, ft_strlen(line));
	if (ret == 0)
		return (input);
	if (input)
		chr_free(&input);
	return (NULL);
}

t_chr			*gnl_nsave_chr(int fd, int nbr_lines)
{
	t_chr		*input;
	char		*line;
	int			ret;
	int			i;

	i = 0;
	ret = 0;
	input = NULL;
	while ((nbr_lines == -1 || i < nbr_lines) &&
			((ret = get_next_line(fd, &line)) > 0))
	{
		chr_addnode(&input, line, ft_strlen(line));
		i++;
	}
	if (ret == 0 || i == nbr_lines)
		return (input);
	if (input)
		chr_free(&input);
	return (NULL);	
}

void			free_intmatrix(int ***tab, int size)
{
	int			i;

	i = 0;
	while (i < size)
	{
		free((*tab)[i]);
		i++;
	}
	free(*tab);
}

void			free_tabhash(char ***tab_hash, int size)
{
	int			i;

	i = 0;
	while (i < size)
		free((*tab_hash)[i++]);
	free(*tab_hash);
}

void			free_infos(t_infos *infos)
{
	int			i;

	if (infos->input)
		chr_free(&(infos->input));
	if (infos->tab_hash)
		free_tabhash(&infos->tab_hash, infos->v);
	if (infos->rooms)
		free(infos->rooms);
	if (infos->links)
		free_intmatrix(&(infos->links), infos->v);
	if (infos->tab_ants)
	{
		i = -1;
		while (++i < infos->ants)
			free(infos->tab_ants[i].tab_life);
		free(infos->tab_ants);
	}
}

void			free_error(t_infos *infos)
{
	free_infos(infos);
	ft_putstr("ERROR\n");
	exit(1);
}

void			init_infos(t_infos *infos)
{
	infos->v = 0;
	infos->ants = 0;
	infos->shots = 0;
	infos->input = NULL;
	infos->tab_hash = NULL;
	infos->rooms = NULL;
	infos->links = NULL;
	infos->tab_ants = NULL;
}

int				is_strnbr(char *str)
{
	int			i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int				is_room(char *str)
{
	int			i;

	i = -1;
	while (str[++i] && str[i] != ' ')
		if (!ft_isalnum(str[i]))
			return (0);
	if (str[i] != ' ')
		return (0);
	while (str[++i] && str[i] != ' ')
		if (!ft_isdigit(str[i]))
			return (0);
	if (str[i] != ' ')
		return (0);
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return ((str[i]) ? 0 : 1);
}

void			find_start_end(t_chr *curr, char *str)
{
	if (!ft_strcmp(str, "start"))
		curr->next->len = INT_MAX;
	else if (!ft_strcmp(str, "end"))
		curr->next->len = UINT_MAX;
}

void			fill_basic_infos(t_infos *infos)
{
	t_chr		*curr;	
	char		*str;

	curr = infos->input;
	if (!ft_strncmp(curr->str, "ERROR", 5))
		free_error(infos);
	while (curr)
	{
		str = curr->str;
		if (str[0] == '#')
		{
			if (str[1] != '#')
				ft_putendl(&str[1]);
			else
				find_start_end(curr, &str[2]);
		}
		else if (is_strnbr(str) && !infos->ants)
			infos->ants = ft_atoi(str);
		else if (is_room(str))
			infos->v++;
		else if (str[0] == 'L')
			infos->shots++;
		curr = curr->next;
	}
}

int			the_color(char *str, int def)
{
	if (!ft_strcmp(str, "RED"))
		return (L_RED);
	if (!ft_strcmp(str, "BLUE"))
		return (L_BLUE);
	if (!ft_strcmp(str, "YELLOW"))
		return (L_YELLOW);
	if (!ft_strcmp(str, "GREEN"))
		return (L_GREEN);
	if (!ft_strcmp(str, "CYAN"))
		return (L_CYAN);
	if (!ft_strcmp(str, "ORANGE"))
		return (L_ORANGE);
	return (def);
}			

int				correct_syntax(char **s)
{
	char		*str;
	int			eol;
	int			i;

	i = 0;
	str = *s;
	while (str[i] && str[i] != '-')
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	eol = i;
	while (str[++i])
		if (str[i] < 65 && str[i] > 90 )
			return (0);
	(*s)[eol] = '\0';
	return (eol);
}

static void		set_ant_color(t_ant_infos *tab_ants, char *str)
{
	int			eol;
	int			n;

	eol = correct_syntax(&str);
	if (eol > 0)
	{
		n = ft_atoi(str);
		tab_ants[n].color = the_color(&str[eol + 1], L_BLACK);
	}
}

static void		set_room_color(t_room *rooms, char *str)
{
	int			eol;
	int			n;

	eol = correct_syntax(&str);
	if (eol > 0)
	{
		n = ft_atoi(str);
		rooms[n].color = the_color(&str[eol + 1], L_WHITE);
	}
}

void			take_cmds(t_infos *infos, t_chr *curr)
{
	if (!ft_strncmp(curr->str, "##color branch ", 15))
		infos->color_paths = the_color(&(curr->str[15]), L_WHITE);
	else if (!ft_strncmp(curr->str, "##color ant ", 12))
		set_ant_color(infos->tab_ants, &(curr->str[12]));
	else if (!ft_strncmp(curr->str, "##color room ", 13))
		set_room_color(infos->rooms, &(curr->str[13]));
}

void			fill_room(t_infos *infos, char *str)
{
	char	**tab;
	int		ind;

	tab = ft_strsplit(str, ' ');
	ind = hash_findid(infos->tab_hash, infos->v, tab[0]);
	infos->rooms[ind].id = ind;
	infos->rooms[ind].coord.x = ft_atoi(tab[1]);
	infos->rooms[ind].coord.y = ft_atoi(tab[2]);
	free_tabstr(&tab);
}

int				is_link(char *str)
{
	int			i;

	i = 0;
	while (str[i] > 32 && str[i] != '-')
		i++;
	if (str[i++] != '-')
		return (0);
	while (str[i] > 32 && str[i] != '-')
		i++;
	if (str[i])
		return (0);
	return (1);
}

void			add_link(char **tab_hash, int v, int **matrix, char *str)
{
	char		**tab;
	int			a;
	int			b;

	tab = ft_strsplit(str, '-');
	a = hash_findid(tab_hash, v, tab[0]);
	b = hash_findid(tab_hash, v, tab[1]);
	matrix[a][b] = 1;
	matrix[b][a] = 1;
}

static int		modify_l(char **str)
{
	char		*s;
	int			i;

	i = 1;
	s = *str;
	while(s[i] && s[i] != '-')
		i++;
	(*str)[i] = '\0';
	return (i);
}

void			fill_tab_ants(t_infos *infos, t_ant_infos *tab_ants, char *str, int time)
{
	char		**tab_cmds;
	int			num_ant;
	int			id_room;
	int			eol;
	int			i;

	i = 0;
	tab_cmds = ft_strsplit(str, ' ');
	while (tab_cmds[i])
	{
		eol = modify_l(&tab_cmds[i]);
		num_ant = ft_atoi(&(tab_cmds[i][1]));
		id_room = hash_findid(infos->tab_hash, infos->v, &tab_cmds[i][eol + 1]);
		tab_ants[num_ant - 1].tab_life[time] = id_room;
		i++;
	}
	free_tabstr(&tab_cmds);
}

void			fill_adv_infos(t_infos *infos)
{
	t_chr		*curr;
	int			time;

	curr = infos->input;
	while (curr)
	{
		if (!ft_strncmp(curr->str, "##", 2))
			take_cmds(infos, curr);
		else if (is_room(curr->str))
			fill_room(infos, curr->str);
		else if (is_link(curr->str))
			add_link(infos->tab_hash, infos->v, infos->links, curr->str);
		else if (curr->str[0] == 'L')
			break ;
		curr = curr->next;
	}
	time = 0;
	while (curr)
	{
		fill_tab_ants(infos, infos->tab_ants, curr->str, time);
		curr = curr->next;
		time++;
	}
}

static int			alloc_tab_ants(t_infos *infos)
{
	int				nshots;
	int				ants;
	int				i;

	ants = infos->ants;
	nshots = infos->shots;
	if (!(infos->tab_ants = (t_ant_infos*)malloc(sizeof(t_ant_infos) * ants)))
		return (0);
	i = 0;
	while (i < ants)
	{
		if (!((infos->tab_ants)[i].tab_life = (int*)malloc(sizeof(int) * nshots)))
			return (0);
		reset_tab_int(infos->tab_ants[i].tab_life, nshots, -1);
		i++;
	}
	return (1);
}

int			alloc_places(t_infos *infos)
{
	int		v;
	int		i;

	v = infos->v;
	if (!(infos->rooms = (t_room*)malloc(sizeof(t_room) * v)))
		return (0);
	if (!(infos->links = (int**)malloc(sizeof(int*) * v)))
		return (0);
	i = 0;
	while (i < v)
	{
		if (!(infos->links[i] = (int*)ft_memalloc(sizeof(int) * v)))
			return(0);
		i++;
	}
	alloc_tab_ants(infos);
	return (1);
}

void			fill_infos(t_infos *infos)
{
	fill_basic_infos(infos);
	if (!alloc_places(infos))
	{
		free_infos(infos);
		exit(1);
	}
	create_tabhash_2(infos, infos->input);
	fill_adv_infos(infos);
}

int				main(void)
{
	t_infos		infos;

	while (1)
	{
		init_infos(&infos);
		if (!(infos.input = gnl_save_chr(0)))
			return(1);
		chr_print(infos.input);			//!!!!!!!!!!!!!!!!!!
		ft_putstr("\n\n");				//!!!!!!!!!!!!!!!!!!
		fill_infos(&infos);
		print_infos(infos);				//!!!!!!!!!!!!!!!!!!
		free_infos(&infos);
	}
	return (0);
}
