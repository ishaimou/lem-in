#include "displayer.h"

int		gnl_print_comment(char **line)
{
	if (get_next_line(0, line) < 0)
		exit(1);
	if (ret == 0)
	{
		free(*line);
		return (0);
	}
	if ((*line)[0] == '#')
	{
		if (*line)[1] != '#')
		{
			ft_putendl(*line);
			free(*line);
			gnl_print_comment(line);
		}
		else
			return (2);
	}
	return (1);
}
