#include "displayer.h"

static void		write_limits(t_display *display, int is_start, t_point c)
{
	SDL_Texture	*texture;
	TTF_Font	*font;
	SDL_Rect	pos;
	char		*name;

	font = TTF_OpenFont(FONT_TYPE_TXT, display->block / 8);
	name = (is_start) ? "start" : "end";
	texture = ttf_texture(display->env.render, font, name, setcolor_sdl(0, 0, 0, 1));
	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
	pos.y = c.y + display->block / 8;
	pos.x = c.x - pos.w / 2;
	SDL_RenderCopy(display->env.render, texture, NULL, &pos);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

static void		limits_square(t_display *display, t_point c, int r, int is_start)
{
	SDL_Color	colors[2];

	colors[0] = setcolor_sdl(0, 0, 0, 1);
	if (is_start)
		colors[1] = setcolor_sdl(0, 0, 255, 1);
	else
		colors[1] = setcolor_sdl(0, 255, 0, 1);
	drawfillsquare_sdl_c(display->env, colors, c, 2 * r);
	write_limits(display, is_start, c);
}

static void		drawnormal_disk(t_sdlenv env, SDL_Color color, t_point c, int r)
{
	SDL_Color	black;

	black = setcolor_sdl(0, 0, 0, 1);
	drawdisk_sdl(env, color, c, r);
	drawcircle_sdl(env, black, c, r);
}

static void		write_name(t_display *display, char *name, t_point c)
{
	SDL_Texture	*texture;
	TTF_Font	*font;
	SDL_Rect	pos;

	font = TTF_OpenFont(FONT_TYPE_TXT, display->block / 8);
	texture = ttf_texture(display->env.render, font, name, setcolor_sdl(0, 0, 0, 1));
	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
	pos.y = c.y - display->block / 4;
	pos.x = c.x - pos.w / 2;
	SDL_RenderCopy(display->env.render, texture, NULL, &pos);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

void			draw_rooms(t_display *display)
{
	t_infos		infos;
	SDL_Color	color;
	t_point		c;
	int			r;
	int			i;

	i = 0;
	r = display->block / 3;
	infos = display->infos;
	while (i < infos.v)
	{
		color = color_macros(infos.rooms[i].color);
		c = ft_setpoint(display->offset.y +
						infos.rooms[i].coord.y * display->block,
						display->offset.x +
						infos.rooms[i].coord.x * display->block);
		if (infos.rooms[i].id == infos.start)
			limits_square(display, c, r, 1);
		else if (infos.rooms[i].id == infos.end)
			limits_square(display, c, r, 0);
		else
			drawnormal_disk(display->env, color, c, r);
		write_name(display, infos.tab_hash[infos.rooms[i].id], c);
		i++;
	}
}
