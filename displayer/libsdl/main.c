#include "libsdl.h"

void		ant_man(t_sdlenv env, SDL_Color color, t_point origin, int g_r, int teta)
{
	t_point	c[4];
	t_point	rot_c[4];
	t_point	p[2];
	int		r;
	int		i;

	r = g_r / 8;
	c[0] = pt_new(origin.y, origin.x - 3 * r);
	c[1] = pt_new(origin.y, origin.x - r);
	c[2] = pt_new(origin.y, origin.x + r);
	c[3] = pt_new(origin.y, origin.x + 3 * r);
	rot_c[0] = pt_rot(origin, c[0], teta);
	rot_c[1] = pt_rot(origin, c[1], teta);
	rot_c[2] = pt_rot(origin, c[2], teta);
	rot_c[3] = pt_rot(origin, c[3], teta);
	p[0] = pt_rot(origin, pt_new(c[0].y + 3 * r / 2, c[0].x - 3 * r / 2), teta);
	p[1] = pt_rot(origin, pt_new(c[0].y - 3 * r / 2, c[0].x - 3 * r / 2), teta);
	sdl_line(env, sdl_rgb(0, 0, 0), rot_c[0], p[0]);
	sdl_line(env, sdl_rgb(0, 0, 0), rot_c[0], p[1]);
	sdl_disk(env, color, rot_c[0], r);
	sdl_circle(env, sdl_rgb(0, 0, 0), rot_c[0], r);
	i = 1;
	while (i < 4)
	{
		p[0] = pt_rot(origin, pt_new(c[i].y - 2 * r, c[i].x), teta);
		p[1] = pt_rot(origin, pt_new(c[i].y + 2 * r, c[i].x), teta);
		sdl_line(env, sdl_rgb(0, 0, 0), p[0], p[1]);
		sdl_disk(env, color, rot_c[i], r);
		sdl_circle(env, sdl_rgb(0, 0, 0), rot_c[i], r);
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
	ant_man(env, color, pt_new(200, 200), 100, 30);
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
