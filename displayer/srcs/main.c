/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 01:41:34 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/25 10:17:35 by ishaimou         ###   ########.fr       */
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
		else if (is_strnbr(str))
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

static void		begin_state(t_infos *infos)
{
	int			i;
	int			j;

	i = 0;
	while (i < infos->ants)
	{
		j = 0;
		while (j <= infos->shots)
		{
			if (infos->tab_ants[i].tab_life[j] != -1)
				break;
			infos->tab_ants[i].tab_life[j] = infos->start;
			j++;
		}
		i++;
	}
}

void			fill_adv_infos(t_infos *infos)
{
	t_chr		*curr;
	int			time;

	curr = infos->input;
	while (curr)
	{
		if (!ft_strncmp(curr->str, "##", 2))
			take_cmds(infos, curr->str);
		else if (is_room(curr->str))
			fill_room(infos, curr->str);
		else if (is_link(curr->str))
			add_link(infos->tab_hash, infos->v, infos->links, curr->str);
		else if (curr->str[0] == 'L')
			break ;
		curr = curr->next;
	}
	time = 1;
	while (curr)
	{
		fill_tab_ants(infos, infos->tab_ants, curr->str, time);
		curr = curr->next;
		time++;
	}
	begin_state(infos);
}

void			take_options(int ac, char **av, t_infos *infos)
{
	if (ac == 2)
	{
		if (!ft_strcmp(av[1], "-d"))
			infos->debug = 1;
		else
		{
			free_infos(infos);
			ft_dprintf(2, "Usage: ./test [-d]\n");
			ft_dprintf(2, "  -d : enable debug mode\n");
			exit(EXIT_FAILURE);
		}
	}
}

static int		in_start(t_infos *infos, int x)
{
	int			count;
	int			i;

	i = 0;
	count = 0;
	while (i < infos->ants)
	{
		if (infos->tab_ants[i].tab_life[x] != -1 &&
			infos->tab_ants[i].tab_life[x] != infos->start &&
			infos->tab_ants[i].tab_life[x - 1] == infos->start)
			count++;
		i++;
	}
	return (count);
}

static int		in_end(t_infos *infos, int x)
{
	int			count;
	int			i;

	i = 0;
	count = 0;
	while (i < infos->ants)
	{
		if (infos->tab_ants[i].tab_life[x] == infos->end)
			count++;
		i++;
	}
	return (count);
}

void			fill_start_end(t_infos *infos)
{
	int			start;
	int			end;
	int			i;

	i = 0;
	end = 0;
	start = infos->ants;
	while (i <= infos->shots)
	{
		end += in_end(infos, i);
		start -= in_start(infos, i);
		infos->start_end[i].y = end;
		infos->start_end[i].x = start;
		i++;
	}
}

void			fill_trace(t_infos *infos)
{
	int			*life;
	int			i;
	int			j;

	i = 0;
	while (i < infos->ants)
	{
		j = 0;
		while (j < infos->shots)
		{
			life = infos->tab_ants[i].tab_life;
			if (life[j] != -1 && life[j + 1] != -1 &&
				life[j] != life[j + 1])
			{
				if (infos->links[life[j]][life[j + 1]] == 2)
					break ;
				infos->links[life[j]][life[j + 1]] = 2;
				infos->links[life[j + 1]][life[j]] = 2;
			}
			j++;
		}
		i++;
	}
}

int				store_data(int ac, char **av, t_infos *infos)
{
	init_infos(infos);
	take_options(ac, av, infos);
	if (!(infos->input = gnl_save_chr(0)))
		return(0);
	chr_print(infos->input);
	ft_putstr("\n\n");
	fill_basic_infos(infos);
	if (!alloc_places(infos))
	{
		free_infos(infos);
		exit(0);
	}
	create_tabhash_2(infos, infos->input);
	fill_adv_infos(infos);
	fill_start_end(infos);
	fill_trace(infos);
	if (infos->debug)
		print_debug(infos);
	return (1);
}

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

static void		draw_link(t_display *display, t_infos infos, int a, int b)
{
	SDL_Color	color;
	t_room		*rooms;
	t_point		p_a;
	t_point		p_b;
	t_bline		bline;
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
	p_a = pt_new(display->offset.y + rooms[i].coord.y * display->block,
			display->offset.x + rooms[i].coord.x * display->block);
	p_b = pt_new(display->offset.y + rooms[j].coord.y * display->block,
			display->offset.x + rooms[j].coord.x * display->block);
	bline = bline_new(p_a, p_b, display->block / 8);
	if (display->infos.links[a][b] == 2 && display->trace)
		color = sdl_rgb(255, 177, 51);
	else
		color = color_macros(infos.color_paths);
	sdl_bline(display->env, color, bline);
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
			if (matrix[i][j])
				draw_link(display, display->infos, i, j);
			j++;
		}
		i++;
	}
}

void			draw_scene(t_display *display)
{
	SDL_Texture		*texture;
	SDL_Rect		rect;

	texture = img_texture(display->env.render, IMG_PATH);
	SDL_RenderCopy(display->env.render, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	rect = rect_new(HEIGHT / 8, WIDTH / 5, HEIGHT / 30, WIDTH / 30);
	SDL_SetRenderDrawColor(display->env.render, 160, 160, 160, 255);
	SDL_RenderFillRect(display->env.render, &rect);
	draw_edge(display);
	draw_rooms(display);
}

int				get_teta(t_infos infos, int a, int b)
{
	t_point		p[2];
	float		rad;

	if (b == -1 || a == infos.end)
		return (0);
	p[0] = infos.rooms[a].coord;
	p[1] = infos.rooms[b].coord;
	rad = atan2(p[1].y - p[0].y, p[1].x - p[0].x);
	if (p[0].y >= p[1].y || p[0].x >= p[1].x)
		rad += M_PI;
	return ((rad * 180) / M_PI);
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
