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
		free_intmatrix(&(infos->links));
	if (infos->tab_ants)
	{
		i = -1;
		while (++i < infos->ants)
			free(infos->tab_ants[i].tab_life);
		free(infos->tab_ants);
	}
}

void			farm_error(t_infos *infos)
{
	free_infos(infos);
	ft_putstr("ERROR\n");
	exit(1);
}

void			init_infos(t_infos *infos)
{
	infos->>v = 0;
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
		farm_error(infos);
	while (curr)
	{
		str = curr->str;
		if (str[0] == '#' && str[1] != '#')
			ft_putendl(&str[1]);
		else if (is_strnbr(str))
			infos->ants = ft_atoi(str);
		else if (is_room(str))
			infos->v++;
		else if (str[0] == 'L')
			infos->shots++;
		curr = curr->next;
	}
}

static int		the_color(char *str, int default)
{
	if (!ft_strcmp(str, "RED"))
		return (RED);
	if (!ft_strcmp(str, "BLUE"))
		return (BLUE);
	if (!ft_strcmp(str, "YELLOW"))
		return (YELLOW);
	if (!ft_strcmp(str, "GREEN"))
		return (GREEN);
	if (!ft_strcmp(str, "CYAN"))
		return (CYAN);
	if (!ft_strcmp(str, "ORANGE"))
		return (ORANGE);
	return (default);
}			

static int		is_ant_color(char **str)
{
	int			eol;
	char		*s;
	int			i;

	i = 0;
	s = *str;
	while (s[i] && s[i] != '-')
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	eol = i;
	i++;
	while (s[i])
	{
		if (s[i] < 65 && s[i] > 90 )
			return (0);
		i++;
	}
	(*str)[eol] = '\0';
	return (eol);
}

static void		set_ant_color(t_ant_infos *tab_ants, char *str)
{
	int			eol;
	int			n;

	eol = is_ant_color(&str);
	if (eol > 0)
	{
		n = ft_atoi(str);
	}
}

void			take_cmds(t_infos *infos, t_chr *curr)
{
	if (!ft_strcmp(curr->str, "##start"))
		curr->next->len = -1;
	else if (!ft_strcmp(curr->str, "##end"))
		curr->next->len = -2;
	else if (!ft_strncmp(curr->str, "##color branch ", 15)
		infos->color_paths = the_color(&(curr->str[15]));
	else if (!ft_strncmp(curr->str, "##color ant ", 12)
		set_ant_color(infos->tab_ants, &(curr->str[12]));
	else if (!ft_strncmp(curr->str, "##color room ", 13)
		set_room_color(infos->rooms, &(curr->str[13]));
}

void			fill_adv_infos(t_infos *infos)
{
	t_chr		*curr;

	curr = infos->input;
	whilr (curr)
	{
		if (!ft_strncmp(curr->str, "##", 2))
			take_cmds(infos, &curr);
		curr = curr-next;
	}
}

static t_ant_infos	*alloc_tab_ants(int ants, int shots)
{
	t_ant_infos		*tab_ants;

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
}

int			alloc_places(t_infos *infos)
{
	int		v;

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
}

void			fill_infos(t_infos *infos)
{
	fill_basic_infos(infos);
	if (!alloc_places(infos))
	{
		free_infos(infos);
		exit(1);
	}
	fill_adv_infos(infos);
}

int				main(void)
{
	t_infos		infos;
	char		**line;

	init_infos(&infos);
	if (!(infos.input = gnl_save_chr(0)))
		return(1);
			chr_print(infos.input);			//!!!!!!!!!!!!!!!!!!
			ft_putstr("\n\n");				//!!!!!!!!!!!!!!!!!!
	fill_infos(&infos);
	return (0);
}
