#include "displayer.h"

static int		correct_syntax(char **s)
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
