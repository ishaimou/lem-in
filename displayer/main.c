/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 01:41:34 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/05 12:10:39 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

static void		find_start_end(t_chr *curr, char *str)
{
	if (!ft_strcmp(str, "start"))
		curr->next->len = INT_MAX;
	else if (!ft_strcmp(str, "end"))
		curr->next->len = UINT_MAX;
}

void			fill_basic_infos(t_infos *infos)
{
	t_chr		*curr;	
	char		*str;

	curr = infos->input;
	if (!ft_strncmp(curr->str, "ERROR", 5))
		free_error(infos);
	while (curr)
	{
		str = curr->str;
		if (str[0] == '#')
		{
			if (str[1] != '#')
				ft_putendl(&str[1]);
			else
				find_start_end(curr, &str[2]);
		}
		else if (is_strnbr(str) && !infos->ants)
			infos->ants = ft_atoi(str);
		else if (is_room(str))
			infos->v++;
		else if (str[0] == 'L')
			infos->shots++;
		curr = curr->next;
	}
}

void			fill_room(t_infos *infos, char *str)
{
	char	**tab;
	int		ind;

	tab = ft_strsplit(str, ' ');
	ind = hash_findid(infos->tab_hash, infos->v, tab[0]);
	infos->rooms[ind].id = ind;
	infos->rooms[ind].coord.x = ft_atoi(tab[1]);
	infos->rooms[ind].coord.y = ft_atoi(tab[2]);
	free_tabstr(&tab);
}

void			add_link(char **tab_hash, int v, int **matrix, char *str)
{
	char		**tab;
	int			a;
	int			b;

	tab = ft_strsplit(str, '-');
	a = hash_findid(tab_hash, v, tab[0]);
	b = hash_findid(tab_hash, v, tab[1]);
	matrix[a][b] = 1;
	matrix[b][a] = 1;
}

static int		modify_l(char **str)
{
	char		*s;
	int			i;

	i = 1;
	s = *str;
	while(s[i] && s[i] != '-')
		i++;
	(*str)[i] = '\0';
	return (i);
}

void			fill_tab_ants(t_infos *infos, t_ant_infos *tab_ants, char *str, int time)
{
	char		**tab_cmds;
	int			num_ant;
	int			id_room;
	int			eol;
	int			i;

	i = 0;
	tab_cmds = ft_strsplit(str, ' ');
	while (tab_cmds[i])
	{
		eol = modify_l(&tab_cmds[i]);
		num_ant = ft_atoi(&(tab_cmds[i][1]));
		id_room = hash_findid(infos->tab_hash, infos->v, &tab_cmds[i][eol + 1]);
		tab_ants[num_ant - 1].tab_life[time] = id_room;
		i++;
	}
	free_tabstr(&tab_cmds);
}

void			fill_adv_infos(t_infos *infos)
{
	t_chr		*curr;
	int			time;

	curr = infos->input;
	while (curr)
	{
		if (!ft_strncmp(curr->str, "##", 2))
			take_cmds(infos, curr);
		else if (is_room(curr->str))
			fill_room(infos, curr->str);
		else if (is_link(curr->str))
			add_link(infos->tab_hash, infos->v, infos->links, curr->str);
		else if (curr->str[0] == 'L')
			break ;
		curr = curr->next;
	}
	time = 0;
	while (curr)
	{
		fill_tab_ants(infos, infos->tab_ants, curr->str, time);
		curr = curr->next;
		time++;
	}
}

int				store_data(t_infos *infos)
{
	init_infos(infos);
	if (!(infos->input = gnl_save_chr(0)))
		return(0);
				chr_print(infos->input);	//!!!!!!!!!!!!!!!!!!
				ft_putstr("\n\n");			//!!!!!!!!!!!!!!!!!!
	fill_basic_infos(infos);
	if (!alloc_places(infos))
	{
		free_infos(infos);
		exit(0);
	}
	create_tabhash_2(infos, infos->input);
	fill_adv_infos(infos);
				print_infos(*infos);		//!!!!!!!!!!!!!!!!!!
	//free_infos(infos);
	return (1);
}

void			print_usage(void)
{
	ft_putstr("Welcome\n");
}

int				init_display(t_display *display)
{
	init_sdlenv(&(display->env), 1, 1, 0);
	init_sdl(display->env);
	if (!fill_sdlenv(&(display->env), TITLE, HEIGHT, WIDTH))
		return (0);
	//load_music(display->env, MUSIC_PATH);
	display->start_ants = display->infos.ants;
	display->end_ants = 0;
	display->moment = 0;
	display->step = 1;
	display->pause = 0;
	display->pass = 1;
	display->block = WIDTH / 10;
	display->color_text = setcolor_sdl(0, 0, 0, 0);
	display->offset = ft_setpoint(HEIGHT / 4, 2 * display->block);
	display->font_text = TTF_OpenFont(FONT_TYPE_TXT, FONT_SIZE_TXT);
	return (1);
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

SDL_Texture		*ttf_texture(SDL_Renderer *render, TTF_Font *font, char *str, SDL_Color color)
{
	SDL_Surface	*surface;
	SDL_Texture	*text;

	surface = TTF_RenderText_Blended(font, str, color);
	text = SDL_CreateTextureFromSurface(render, surface);
	return (text);
}

void			display_shots(t_display *display)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Done in ", display->moment);
	ft_strcombin(&str, " shots");
	tex = ttf_texture(display->env.render, display->font_text,
						str, display->color_text);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 10;
	pos.x = 20;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	free(str);
}

void			display_ants(t_display *display)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Start: ", display->start_ants);
	tex = ttf_texture(display->env.render, display->font_text,
						str, display->color_text);
	pos = create_rect(0, 0, HEIGHT / 30 + 5, 20);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	str = str_msg("End: ", display->end_ants);
	tex = ttf_texture(display->env.render, display->font_text,
						str, display->color_text);
	pos = create_rect(0, 0, HEIGHT / 30 + 5, 150);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 30 + 5;
	pos.x = WIDTH / 30 + 200 - pos.w;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
}

SDL_Color		color_macros(int macros)
{
	if (macros == L_YELLOW)
		return (setcolor_sdl(255, 255, 0, 1));
	if (macros == L_WHITE)
		return (setcolor_sdl(255, 255, 255, 1));
	if (macros == L_BLACK)
		return (setcolor_sdl(0, 0, 0, 1));
	if (macros == L_GREEN)
		return (setcolor_sdl(0, 255, 0, 1));
	if (macros == L_RED)
		return (setcolor_sdl(255, 0, 0, 1));
	if (macros == L_BLUE)
		return (setcolor_sdl(0, 0, 255, 1));
	if (macros == L_ORANGE)
		return (setcolor_sdl(255, 70, 0, 1));
	if (macros == L_CYAN)
		return (setcolor_sdl(0, 255, 255, 1));
	if (macros == L_MAGENTA)
		return (setcolor_sdl(255, 0, 255, 1));
	return (setcolor_sdl(0, 0, 0, 1));
}

static void		draw_link(t_display *display, t_infos infos, int a, int b)
{
	t_room		*rooms;
	t_point		p_a;
	t_point		p_b;
	t_bline		bold;
	int			i;
	int			j;
	int			k;

	k = 0;
	rooms = infos.rooms;
	while (k < infos.v)
	{
		if (rooms[k].id == a)
			i = k;
		if (rooms[k].id == b)
			j = k;
		k++;
	}
	p_a = ft_setpoint(display->offset.y + rooms[i].coord.y * display->block,
					display->offset.x + rooms[i].coord.x * display->block);
	p_b = ft_setpoint(display->offset.y + rooms[j].coord.y * display->block,
					display->offset.x + rooms[j].coord.x * display->block);
	bold = ft_setboldline(p_a, p_b, 5);
	drawboldline_sdl(display->env, color_macros(infos.color_paths), bold);
}

static void		draw_edge(t_display *display)
{
	int			**matrix;
	int			size;
	int			i;
	int			j;

	i = 0;
	size = display->infos.v;
	matrix = display->infos.links;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			if (matrix[i][j] == 1)
				draw_link(display, display->infos, i, j);
			j++;
		}
		i++;
	}
}

static void		draw_rooms(t_display *display)
{
	t_infos		infos;
	t_point		c;
	int			r;
	int			i;

	i = 0;
	r = display->block / 3;
	infos = display->infos;
	while (i < infos.v)
	{
		c = ft_setpoint(display->offset.y + infos.rooms[i].coord.y * display->block,
					display->offset.x + infos.rooms[i].coord.x * display->block);
		if (infos.rooms[i].id == infos.start)
			;
		else if (infos.rooms[i].id == infos.end)
			;
		else
			drawdisk_sdl(display->env, color_macros(infos.rooms[i].color), c, r);
		i++;
	}
}

void			display_graph(t_display *display)
{
	draw_edge(display);
	draw_rooms(display);
}

void			display_vars(t_display *display)
{
	display_shots(display);
	display_ants(display);
}

void			draw_scene(t_display *display)
{
	SDL_Texture		*texture;
	SDL_Rect		rect;

	texture = texture_img(display->env.render, IMG_PATH);
	SDL_RenderCopy(display->env.render, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	rect = create_rect(100, 200, HEIGHT / 30, WIDTH / 30);
	SDL_SetRenderDrawColor(display->env.render, 160, 160, 160, 255);
	SDL_RenderFillRect(display->env.render, &rect);
	display_graph(display);
	//SDL_RenderPresent(display->env.render);
}

void			draw_ant(t_display *display, t_infos infos, int x)
{
	t_point		coord_ant;
	SDL_Color	color;
	int			v_now;

	v_now = infos.tab_ants[x].tab_life[display->moment];
	color = color_macros(infos.tab_ants[x].color);
	coord_ant = ft_setpoint(display->offset.y + infos.rooms[v_now].coord.y * display->block,
					display->offset.x + infos.rooms[v_now].coord.x * display->block);
	drawdisk_sdl(display->env, color, coord_ant, display->block / 8);
}

void			draw_state(t_display *display, t_infos infos)
{
	int			ant_state;
	int			i;

	if (display->moment > infos.shots)
		return ;
	i = 0;
	draw_scene(display);
	while (i < infos.ants)
	{
		ant_state = infos.tab_ants[i].tab_life[display->moment];
		if (ant_state != -1)
		{
			if (!infos.tab_ants[i].out)
			{
				display->start_ants--;
				infos.tab_ants[i].out = 1;
			}
			if (ant_state == infos.end)
				display->end_ants++;
			draw_ant(display, infos, i);
		}
		i++;
	}
	display_vars(display);
	SDL_RenderPresent(display->env.render);
	display->moment++;
}

void			event_keydown(t_display *display, SDL_Event event)
{
	if (event.key.keysym.sym == SDLK_ESCAPE)
		display->pass = 0;
	if (event.key.keysym.sym == SDLK_SPACE)
		display->pause = (display->pause) ? 0 : 1;
}

void			free_display(t_display *display)
{
	TTF_CloseFont(display->font_text);
	free_infos(&(display->infos));
	free_sdl(&(display->env));
}

int				main(void)
{
	t_display	display;
	SDL_Event	event;

	print_usage();
	if (!store_data(&(display.infos)))
		return(EXIT_FAILURE);
	if (!init_display(&display))
		return(EXIT_FAILURE);
	draw_scene(&display);
	display_ants(&display);
	while (display.pass)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				display.pass = 0;
			if (event.type == SDL_KEYDOWN)
				event_keydown(&display, event);
			if (!display.pause)
			{
				draw_state(&display, display.infos);
				SDL_Delay(5);
			}
		}
	}
	free_display(&display);
	return (EXIT_SUCCESS);
}
