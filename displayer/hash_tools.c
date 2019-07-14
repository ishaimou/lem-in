#include "displayer.h"

static char	*name_vertex(char *str)
{
	char	*s;
	int		i;

	i = 0;
	s = ft_strdup(str);
	while (s[i] && s[i] != ' ')
		i++;
	s[i] = '\0';
	return (s);
}

void		create_tabhash_2(t_infos *infos, t_chr *list)
{
	char	*s;
	int		ind;
	int		i;

	if (!(infos->tab_hash = (char**)malloc(sizeof(char*) * (infos->v))))
		free_error(infos);
	i = 0;
	while (i < infos->v)
		(infos->tab_hash)[i++] = NULL;
	while (list)
	{
		if (!is_room(list->str))
		{
			list = list->next;
			continue ;
		}
		s = name_vertex(list->str);
		ind = hash_strtoind(infos->tab_hash, infos->v, s);
		hash_putintab(infos->tab_hash, infos->v, s, &ind);
		if (list->len == INT_MAX)
			infos->start = ind;
		if (list->len == UINT_MAX)
			infos->end = ind;
		list = list->next;
		free(s);
	}
}
