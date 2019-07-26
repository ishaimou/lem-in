/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 01:41:34 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/26 02:11:08 by ishaimou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

void			print_usage(void)
{
	ft_printf("%{blue}=====================|%{BLUE} USAGE %{blue}|");
	ft_printf("==================%{eoc}\n");
	ft_printf("        %{GREEN}* Displayer commands:%{eoc}\n");
	ft_printf("   Exit        :  %{CYAN}ESCAPE%{eoc}\n");
	ft_printf("   Move        :  %{CYAN}ARROWS%{eoc}\n");
	ft_printf("   Pause       :  %{CYAN}SPACE%{eoc}\n");
	ft_printf("   Reset       :  %{CYAN}R%{eoc}\n");
	ft_printf("   Zoom In     :  %{CYAN}+%{eoc}\n");
	ft_printf("   Zoom Out    :  %{CYAN}-%{eoc}\n");
	ft_printf("   Next Shot   :  %{CYAN}F%{eoc}\n");
	ft_printf("   Prev Shot   :  %{CYAN}B%{eoc}\n");
	ft_printf("   Speed Up    :  %{CYAN}2%{eoc}\n");
	ft_printf("   Speed Down  :  %{CYAN}1%{eoc}\n");
	ft_printf("   Trace On/Off:  %{CYAN}T%{eoc}\n");
	ft_printf("   Music On/Off:  %{CYAN}M%{eoc}\n");
	ft_printf("\n        %{GREEN}* Map commands:%{eoc}\n");
	ft_printf("  %{cyan}##color room %{eoc}room_name%{cyan}-%{eoc}color\n");
	ft_printf("  %{cyan}##color ant %{eoc}ant_num%{cyan}-%{eoc}color\n");
	ft_printf("\n        %{GREEN}* Colors:%{eoc}\n");
	ft_printf(" %{RED}RED %{BLUE}BLUE %{GREEN}GREEN %{YELLOW}YELLOW");
	ft_printf(" %{CYAN}CYAN %{eoc}ORANGE %{PURPLE}PURPLE\n");
	ft_printf("%{blue}=================================================%{eoc}\n");
}

static char		*str_msg(char *msg, int nbr)
{
	char		*str;
	char		*tmp;

	str = ft_strdup(msg);
	tmp = ft_itoa(nbr);
	ft_strcombin(&str, tmp);
	free(tmp);
	return (str);
}

void			display_shots(t_display *display)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Shots: ", display->moment / STATE);
	tex = ttf_texture(display->env.render, display->font_text,
			str, sdl_rgb(0, 0, 0));
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 10;
	pos.x = WIDTH / 30 + WIDTH / 10 - pos.w / 2;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	free(str);
}

void			display_ants(t_display *display)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Start: ", display->infos.start_end[display->moment / STATE].x);
	tex = ttf_texture(display->env.render, display->font_text,
			str, sdl_rgb(0, 0, 0));
	pos = rect_new(0, 0, HEIGHT / 20, WIDTH / 20);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	str = str_msg("End: ", display->infos.start_end[display->moment / STATE].y);
	tex = ttf_texture(display->env.render, display->font_text,
			str, sdl_rgb(0, 0, 0));
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 20;
	pos.x = WIDTH / 60 + WIDTH / 5 - pos.w;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
}

int				get_teta(t_infos infos, int a, int b)
{
	t_point		p[2];
	float		teta;

	if (b == -1 || a == infos.end)
		return (0);
	p[0] = infos.rooms[a].coord;
	p[1] = infos.rooms[b].coord;
	teta = pt_diffteta(p[0], p[1]);
	if (p[0].x < p[1].x)
		teta += 180;
	return (teta);
}

void			draw_ant(t_display *display, t_infos infos, int x)
{
	t_point		coord_ant;
	SDL_Color	color;
	int			v_now;
	int			v_next;
	t_point		p1;
	t_point		p2;

	v_next = -1;
	v_now = infos.tab_ants[x].tab_life[display->moment / STATE];
	color = color_macros(infos.tab_ants[x].color);
	p1 = pt_new(display->offset.y +
			infos.rooms[v_now].coord.y * display->block,
			display->offset.x +
			infos.rooms[v_now].coord.x * display->block);
	if (display->moment < STATE * infos.shots)
	{
		v_next = infos.tab_ants[x].tab_life[display->moment / STATE + 1];
		if (v_next != -1)
		{
			p2 = pt_new(display->offset.y +
					infos.rooms[v_next].coord.y * display->block,
					display->offset.x +
					infos.rooms[v_next].coord.x * display->block);
			coord_ant = pt_linterp(p1, p2, display->moment % STATE, STATE);
		}
		else
			coord_ant = pt_new(p1.y, p1.x);
	}
	else
		coord_ant = pt_new(p1.y, p1.x);
	ant_man(display, color, coord_ant,
			get_teta(display->infos, v_now, v_next));
}

void			draw_full_limit(t_display *display, int is_start)
{
	t_infos		infos;
	t_point		coord_ant;
	SDL_Color	color;

	infos = display->infos;
	color = sdl_rgb(0, 0, 0);
	if (is_start)
		coord_ant = pt_new(display->offset.y + infos.rooms[infos.start].coord.y * display->block,
				display->offset.x + infos.rooms[infos.start].coord.x * display->block);
	else
		coord_ant = pt_new(display->offset.y + infos.rooms[infos.end].coord.y * display->block,
				display->offset.x + infos.rooms[infos.end].coord.x * display->block);
	sdl_disk(display->env, color, coord_ant, display->block / 15);
}

void			draw_state(t_display *display, t_infos infos)
{
	int			ant_state;
	int			i;

	i = 0;
	draw_scene(display);
	while (i < infos.ants)
	{
		ant_state = infos.tab_ants[i].tab_life[display->moment / STATE];
		if (ant_state != -1)
			draw_ant(display, infos, i);
		i++;
	}
	display_shots(display);
	display_ants(display);
	SDL_RenderPresent(display->env.render);
}

int				main(int ac, char **av)
{
	t_display	display;

	if (!store_data(ac, av, &(display.infos)))
		return(EXIT_FAILURE);
	if (!init_display(&display))
		return(EXIT_FAILURE);
	print_usage();
	Mix_PlayMusic(display.env.music, display.mute);
	displayer_loop(&display);
	free_display(&display);
	return (EXIT_SUCCESS);
}
