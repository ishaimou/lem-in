#include "libsdl.h"

void		ant_man(t_sdlenv env, SDL_Color color, t_point c, int r)
{
	t_bline	bline;
	t_point	small_c[4];
	t_point	p[2];
	int		small_r;
	int		i;

	small_r = r / 8;
	small_c[0] = pt_new(c.y, c.x - 3 * small_r);
	small_c[1] = pt_new(c.y, c.x - small_r);
	small_c[2] = pt_new(c.y, c.x + small_r);
	small_c[3] = pt_new(c.y, c.x + 3 * small_r);
	p[0] = pt_new(small_c[0].y + 3 * small_r / 2, small_c[0].x - 3 * small_r / 2);
	p[1] = pt_new(small_c[0].y - 3 * small_r / 2, small_c[0].x - 3 * small_r / 2);
	bline = bline_new(small_c[0], p[0], 3);
	sdl_bline(env, sdl_rgb(0, 0, 0), bline);
	bline = bline_new(small_c[0], p[1], 3);
	sdl_bline(env, sdl_rgb(0, 0, 0), bline);
	sdl_disk(env, color, small_c[0], small_r);
	sdl_circle(env, sdl_rgb(0, 0, 0), small_c[i], small_r);
	i = 1;
	while (i < 4)
	{
		p[0] = pt_new(small_c[i].y - 2 * small_r, small_c[i].x);
		p[1] = pt_new(small_c[i].y + 2 * small_r, small_c[i].x);
		bline = bline_new(p[0], p[1], 3);
		sdl_bline(env, sdl_rgb(0, 0, 0), bline);
		sdl_disk(env, color, small_c[i], small_r);
		sdl_circle(env, sdl_rgb(0, 0, 0), small_c[i], small_r);
		i++;
	}
}

int		main(void)
{
	t_sdlenv	env;
	SDL_Color	color;
	SDL_Color	colors[2];
		SDL_Event	event;

	sdlenv_init(&env, 1, 1, 0);
	if (!sdl_init(env))
		return (EXIT_FAILURE);
	if (!sdlenv_fill(&env, "Obelouch", 1200, 800))
		return (EXIT_FAILURE);
	color = sdl_rgb(0, 255, 0);
	colors[0] = sdl_rgb(255, 0, 0);
	colors[1] = sdl_rgb(0, 255, 0);
	sdl_circle(env, color, pt_new(200, 200), 100);
	ant_man(env, color, pt_new(200, 200), 100);
	//drawdisk_sdl(env, color, ft_setpoint(200, 200), 100);
	//drawline_sdl(env, color, ft_setpoint(300, 200), ft_setpoint(100, 200));
	//drawrect_sdl_b(env, color, create_rect(200, 300, 100, 100));
	//drawfillsquare_sdl_c(env, colors, ft_setpoint(100, 100), 200);
	//drawquad_sdl(env, color, 5, ft_setpoint(100, 100), ft_setpoint(100, 200), ft_setpoint(200, 200), ft_setpoint(200, 100), ft_setpoint(50, 50));
	//drawboldquad_sdl(env, color, 10, 5, ft_setpoint(100, 100), ft_setpoint(100, 200), ft_setpoint(200, 200), ft_setpoint(200, 100), ft_setpoint(50, 50));
	//drawboldrect_sdl(env, color, ft_setboldline(ft_setpoint(100, 100), ft_setpoint(300, 300), 10));
	SDL_RenderPresent(env.render);
	int	pass = 1;
		while (pass)
		{
			while (SDL_PollEvent(&event))
				if (event.type == SDL_QUIT)
					pass = 0;
		}
	sdl_close(&env);
	return (0);
}
