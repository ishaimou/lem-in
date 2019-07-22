#include "displayer.h"

void			init_infos(t_infos *infos)
{
	infos->debug = 0;
	infos->v = 0;
	infos->ants = 0;
	infos->shots = 0;
	infos->input = NULL;
	infos->tab_hash = NULL;
	infos->rooms = NULL;
	infos->links = NULL;
	infos->tab_ants = NULL;
	infos->color_paths = L_WHITE;
}

void			free_error(t_infos *infos)
{
	free_infos(infos);
	ft_putstr("ERROR\n");
	exit(1);
}

void			free_infos(t_infos *infos)
{
	int			i;

	if (infos->input)
		chr_free(&(infos->input));
	if (infos->tab_hash)
		hash_freetab(&infos->tab_hash, infos->v);
	if (infos->rooms)
		free(infos->rooms);
	if (infos->links)
		mxint_free(&(infos->links), infos->v);
	if (infos->tab_ants)
	{
		i = -1;
		while (++i < infos->ants)
			free(infos->tab_ants[i].tab_life);
		free(infos->tab_ants);
	}
}
