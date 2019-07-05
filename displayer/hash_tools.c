#include "displayer.h"

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
		ind = str_to_ind(infos->tab_hash, infos->v, s);
		put_in_tabhash(infos->tab_hash, infos->v, s, &ind);
		if (list->len == INT_MAX)
			infos->start = ind;
		if (list->len == UINT_MAX)
			infos->end = ind;
		list = list->next;
		free(s);
	}
}
