#include "displayer.h"

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

int				is_strnbr(char *str)
{
	int			i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return (1);
}
