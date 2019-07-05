#ifndef DISPLAYER_H
# include "libsdl/libsdl.h"
# include "../libft/includes/libft.h"

# define	L_WHITE -1
# define	L_BLACK 0
# define	L_RED	1
# define	L_GREEN 2
# define	L_YELLOW 3
# define	L_BLUE 4
# define	L_ORANGE 5
# define	L_CYAN 6

typedef struct		s_display
{
	t_sdlenv		env;
}					t_display;

typedef struct		s_ant_infos
{
	int				*tab_life;
	int				color;
}					t_ant_infos;

typedef struct		s_room
{
	int				id;
	int				color;
	t_point			coord;
}					t_room;

typedef struct		s_infos
{
	t_chr			*input;
	int				ants;
	int				shots;
	int				v;
	int				start;
	int				end;
	char			**tab_hash;
	t_room			*rooms;
	int				**links;
	t_ant_infos		*tab_ants;
	int				color_paths;
}					t_infos;

void		put_in_tabhash(char **tab_hash, int size, char *str, int *ind);
int			str_to_ind(char **tab_hash, int size, char *str);
int			hash_findid(char **tab_hash, int size, char *str);
void		create_tabhash_2(t_infos *infos, t_chr *list);
void		free_error(t_infos *infos);
int			is_room(char *str);
void		print_infos(t_infos infos);		//!!!!!!!!!!!!!!!!

#endif
