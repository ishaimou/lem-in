#include "lemin.h"

void print_lemin(t_lemin *lemin)
{
	ft_printf("\n+++++++++++++++ lem-in ++++++++++\n");
	ft_printf(" nbr ants   = %d\n", lemin->ants);
	ft_printf(" nbr vertex = %d\n", lemin->v);
	ft_printf(" start = %d\n", lemin->start);
	ft_printf(" end   = %d\n", lemin->end);
	ft_printf("++++++++++++++++++++++++++++++++++\n");
}

void print_tabhash(char **tab)
{
	int		i = 0;

	while (tab[i])
	{
		ft_printf("tab[%d] = %s\n", i, tab[i]);
		i++;
	}
}

void print_tree(void *item)
{
	t_room	*room;

	room = (t_room*)item;
	ft_printf("[%d]-", room->id);
}

void print_tabbt(t_bt **tab_bt)
{
	int		i;

	i = 0;
	while (tab_bt[i])
	{
		ft_printf("\n------------ TREE %d ------------\n", i);
		bt_apply_infix(tab_bt[i], &print_tree);
		i++;
	}
	ft_printf("\n");
}
