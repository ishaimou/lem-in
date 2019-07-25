#include "displayer.h"

void		ant_man(t_sdlenv env, SDL_Color color, t_point c, int r)
{
	t_bline	bline;
	t_point	small_c[4];
	t_point	p[2];
	int		small_r;
	int		i;

	small_r = r / 3;
	small_c[0] = pt_new(c.y, c.x - 3 * small_r);
	small_c[1] = pt_new(c.y, c.x - small_r);
	small_c[2] = pt_new(c.y, c.x + small_r);
	small_c[3] = pt_new(c.y, c.x + 3 * small_r);
	p[0] = pt_new(small_c[0].y + 3 * small_r / 2, small_c[0].x - 3 * small_r / 2);
	p[1] = pt_new(small_c[0].y - 3 * small_r / 2, small_c[0].x - 3 * small_r / 2);
	sdl_line(env, sdl_rgb(0, 0, 0), small_c[0], p[0]);
	sdl_line(env, sdl_rgb(0, 0, 0), small_c[0], p[1]);
	sdl_disk(env, color, small_c[0], small_r);
	sdl_circle(env, sdl_rgb(0, 0, 0), small_c[0], small_r);
	i = 1;
	while (i < 4)
	{
		p[0] = pt_new(small_c[i].y - 2 * small_r, small_c[i].x);
		p[1] = pt_new(small_c[i].y + 2 * small_r, small_c[i].x);
		sdl_line(env, sdl_rgb(0, 0, 0), p[0], p[1]);
		sdl_disk(env, color, small_c[i], small_r);
		sdl_circle(env, sdl_rgb(0, 0, 0), small_c[i], small_r);
		i++;
	}
}
