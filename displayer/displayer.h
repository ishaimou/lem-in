#ifndef DISPLAYER_H
# include "libsdl/libsdl.h"
# include "../libft/includes/libft.h"

# define	WHITE -1
# define	BLACK 0
# define	RED	1
# define	GREEN 2
# define	YELLOW 3
# define	BLUE 4
# define	ORANGE 5
# define	CYAN 6

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

#endif
