#include "lemin.h"

void print_lemin(t_lemin *lemin)
{
	ft_printf("\n+++++++++++++++ lem-in ++++++++++\n");
	ft_printf(" nbr ants   = %d\n", lemin->ants);
	ft_printf(" nbr vertex = %d\n", lemin->v);
	ft_printf(" start = %d\n", lemin->start);
	ft_printf(" end   = %d\n", lemin->end);
	ft_printf(" flux  = %d\n", lemin->flux);
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

void	print_list_grp(t_list *grp)
{
	int		i;

	i = 1;
	while (grp)
	{
		ft_putstr(" ------ GROUP ");
		ft_putchar('[');
		ft_putnbr(i);
		ft_putchar(']');
		ft_putstr(" ------\n");
		print_list_paths((t_list*)grp->content);
		grp = grp->next;
		i++;
		ft_putchar('\n');
	}
}

void	print_list_paths(t_list	*list_paths)
{
	while (list_paths)
	{
		ic_print((t_icase*)(list_paths->content));
		list_paths = list_paths->next;
	}
}

void		print_path(t_lemin *lemin)
{
	int		u;

	u = lemin->end;
	while (u != lemin->start)
	{
		ft_putendl((lemin->tab_hash)[u]);
		u = lemin->parent[u];
	}
		ft_putendl((lemin->tab_hash)[u]);
		ft_putchar('\n');
		ft_putchar('\n');
}
