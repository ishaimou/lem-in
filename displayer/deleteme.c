#include "displayer.h"

static void		print_tabhash(char **tab, int v)
{
	int		i = 0;

	while (i < v)
	{
		ft_printf("tab[%d] = %s\n", i, tab[i]);
		i++;
	}
}

static void		print_rooms(t_room *rooms, int size)
{
	int			i;

	i = 0;
	while (i < size)
	{
		ft_printf("==== room %d ====\n", i);
		ft_printf("id   : %d\n", rooms[i].id);
		ft_printf("color: %d\n", rooms[i].color);
		ft_printf("x: %d\ty: %d\n", rooms[i].coord.x, rooms[i].coord.y);
		ft_putchar('\n');
		i++;
	}
}

static void		print_matrix(int **matrix, int size)
{
	int			i;
	int			j;

	i = -1;
	ft_putstr("  | ");
	while (++i < size)
		ft_printf("%d  ", i);
	ft_putchar('\n');
	i = -1;
	while (++i < size)
		ft_putstr("----");
	ft_putchar('\n');
	i = 0;
	while (i < size)
	{
		j = 0;
		ft_putnbr(i);
		ft_putstr(" | ");
		while (j < size)
		{
			ft_putnbr(matrix[i][j]);
			ft_putstr("  ");
			j++;
		}
		ft_putchar('\n');
		i++;
	}
}

static void		print_tab_ants(t_ant_infos *tab_ants, int ants, int shots)
{
	int			i;
	int			j;

	i = 0;
	while (i < ants)
	{
		printf("------ ant %d -------\n", i + 1);
		printf("color   : %d\n", tab_ants[i].color);
		printf("tab_life:\n");
		j = 0;
		while (j < shots)
			printf(" %d |", tab_ants[i].tab_life[j++]);
		printf("\n");
		i++;
	}
}

void			print_infos(t_infos infos)
{
	print_tabhash(infos.tab_hash, infos.v);
	ft_printf("infos ants: %d\n", infos.ants);
	ft_printf("infos v   : %d\n", infos.v);
	ft_printf("infos shots: %d\n", infos.shots);
	ft_printf("infos start: %d\n", infos.start);
	ft_printf("infos end  : %d\n", infos.end);
	print_rooms(infos.rooms, infos.v);
	ft_putstr("matrix:\n");
	print_matrix(infos.links, infos.v);
	print_tab_ants(infos.tab_ants, infos.ants, infos.shots);
}
