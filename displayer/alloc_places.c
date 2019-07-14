#include "displayer.h"

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
		tabint_reset(infos->tab_ants[i].tab_life, nshots, -1);
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
