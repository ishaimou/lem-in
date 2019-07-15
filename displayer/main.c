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
	free_infos(infos);
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
	display->pause = 0;
	display->pass = 1;
	display->end_ants = 0;
	display->start_ants = display->infos.ants;
	display->color_text = setcolor_sdl(0, 0, 0, 0);
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

void			display_shots(t_display *display, TTF_Font *font)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Done in ", display->infos.shots);
	ft_strcombin(&str, " shots");
	tex = ttf_texture(display->env.render, font, str, display->color_text);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 10;
	pos.x = 20;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	free(str);
}

void			display_ants(t_display *display, TTF_Font *font)
{
	SDL_Texture	*tex;
	SDL_Rect	pos;
	char		*str;

	str = str_msg("Start: ", display->start_ants);
	tex = ttf_texture(display->env.render, font, str, display->color_text);
	pos = create_rect(0, 0, HEIGHT / 30 + 5, 20);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
	str = str_msg("End: ", display->end_ants);
	tex = ttf_texture(display->env.render, font, str, display->color_text);
	pos = create_rect(0, 0, HEIGHT / 30 + 5, 150);
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.y = HEIGHT / 30 + 5;
	pos.x = WIDTH / 30 + 200 - pos.w;
	SDL_RenderCopy(display->env.render, tex, NULL, &pos);
	SDL_DestroyTexture(tex);
}

/*void			draw_bkgraph(t_display *display)
{
	
}*/

void			draw_graph(t_display *display)
{
}

void			draw_scene(t_display *display)
{
	SDL_Texture		*texture;
	SDL_Rect		rect;
	TTF_Font		*font;

	font = TTF_OpenFont(FONT_TYPE_TXT, FONT_SIZE_TXT);
	texture = texture_img(display->env.render, IMG_PATH);
	SDL_RenderCopy(display->env.render, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	rect = create_rect(100, 200, HEIGHT / 30, WIDTH / 30);
	SDL_SetRenderDrawColor(display->env.render, 160, 160, 160, 255);
	SDL_RenderFillRect(display->env.render, &rect);
	display_shots(display, font);
	display_ants(display, font);
	//display_bkgraph(display);
	display_graph(display);
	TTF_CloseFont(font);
}

void			draw_state(t_display *display)
{
	draw_scene(display);
	SDL_RenderPresent(display->env.render);
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
	while (display.pass)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				display.pass = 0;
			//if (event.type == SDL_KEYDOWN)
			//	event_keydown(display);
			if (!display.pause)
			{
				draw_state(&display);
				SDL_Delay(1/60);
			}
		}
	}
	free_sdl(&(display.env));
	return (EXIT_SUCCESS);
}
