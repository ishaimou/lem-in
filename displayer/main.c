/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 01:41:34 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/05 03:18:24 by obelouch         ###   ########.fr       */
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

void			free_infos(t_infos *infos)
{
	int			i;

	if (infos->input)
		chr_free(&(infos->input));
	if (infos->tab_hash)
		free_tabstr(&infos->tab_hash);
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

static int		is_room(char *str)
{
	int			i;

	i = 0;
	while (str[i] && str[i] != ' ')
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (str[i] != ' ')
		return (0);
	return (1);
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
		if (str[0] == '#' && str[1] != '#')
		{
			ft_putendl(&str[1]);
			ft_putstr("=================\n");   //!!!!!!!!!!!!!!!!!!!!
		}
		else if (is_strnbr(str))
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
	if (!ft_strcmp(curr->str, "##start"))
		curr->next->len = 1;
	else if (!ft_strcmp(curr->str, "##end"))
		curr->next->len = 2;
	else if (!ft_strncmp(curr->str, "##color branch ", 15))
		infos->color_paths = the_color(&(curr->str[15]), L_WHITE);
	else if (!ft_strncmp(curr->str, "##color ant ", 12))
		set_ant_color(infos->tab_ants, &(curr->str[12]));
	else if (!ft_strncmp(curr->str, "##color room ", 13))
		set_room_color(infos->rooms, &(curr->str[13]));
}

void			fill_adv_infos(t_infos *infos)
{
	t_chr		*curr;

	curr = infos->input;
	while (curr)
	{
		if (!ft_strncmp(curr->str, "##", 2))
			take_cmds(infos, curr);
		//else if (is_room(curr->str))
		//	fill_room(infos, curr->str);
		curr = curr->next;
	}
}

static t_ant_infos	*alloc_tab_ants(int ants, int shots)
{
	t_ant_infos		*tab_ants;
	int				i;

	if (!(tab_ants = (t_ant_infos*)malloc(sizeof(t_ant_infos) * ants)))
		return (0);
	i = 0;
	while (i < ants)
	{
		if (!(tab_ants[i].tab_life = (int*)malloc(sizeof(int) * shots)))
		{
			free(tab_ants);
			tab_ants = NULL;
			return (0);
		}
		reset_tab_int(tab_ants[i].tab_life, shots, -1);
		i++;
	}
	return (tab_ants);
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
		{
			free(infos->links[i - 1]);
			infos->links[i - 1] = NULL;
			return(0);
		}
		i++;
	}
	infos->tab_ants = alloc_tab_ants(infos->ants, infos->shots);
	return (1);
}

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
		return (0);
	}
	return (1);
}

void		put_in_tabhash(char **tab_hash, int size, char *str, int *ind)
{
	int		i;

	i = *ind;
	while (i < size)
	{
		if (!test_hash(&tab_hash, str, ind, i))
			return ;
		i++;
	}
	i = 0;
	while (i < *ind)
	{
		if (!test_hash(&tab_hash, str, ind, i))
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

void		create_tabhash_2(t_infos *infos, t_chr *list)
{
	int		ind;
	int		i;

	if (!(infos->tab_hash = (char**)malloc(sizeof(char*) * (infos->v + 1))))
		free_error(infos);
	i = 0;
	while (i <= infos->v)
		(infos->tab_hash)[i++] = NULL;
	while (list)
	{
		if (!is_room(list->str))
			continue ;
		ind = str_to_ind(infos->tab_hash, infos->v, list->str);
		put_in_tabhash(infos->tab_hash, infos->v, list->str, &ind);
		if (list->len == 1)
			infos->start = ind;
		if (list->len == 2)
			infos->end = ind;
		list = list->next;
	}
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
	//fill_adv_infos(infos);
}

void			print_infos(t_infos infos)
{
	ft_printf("infos ants: %d\n", infos.ants);
	ft_printf("infos v: %d\n", infos.v);
	ft_printf("infos shots: %d\n", infos.shots);
}

int				main(void)
{
	t_infos		infos;

	init_infos(&infos);
	if (!(infos.input = gnl_save_chr(0)))
		return(1);
	chr_print(infos.input);			//!!!!!!!!!!!!!!!!!!
	ft_putstr("\n\n");				//!!!!!!!!!!!!!!!!!!
	fill_infos(&infos);
	print_infos(infos);
	return (0);
}
