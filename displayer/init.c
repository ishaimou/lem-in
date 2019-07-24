/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 02:16:40 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/24 02:16:42 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

void			init_infos(t_infos *infos)
{
	infos->debug = 0;
	infos->v = 0;
	infos->ants = 0;
	infos->shots = 0;
	infos->input = NULL;
	infos->tab_hash = NULL;
	infos->rooms = NULL;
	infos->links = NULL;
	infos->tab_ants = NULL;
	infos->color_paths = L_WHITE;
}

void			init_vars_display(t_display *display)
{
	display->start_ants = display->infos.ants;
	display->end_ants = 0;
	display->moment = 0;
	display->step = 1;
	display->pause = 1;
	display->pass = 1;
	display->block = WIDTH / 10;
	display->color_text = setcolor_sdl(0, 0, 0, 0);
	display->offset = ft_setpoint(400, 200);
}

int				init_display(t_display *display)
{
	init_sdlenv(&(display->env), 1, 1, 0);
	init_sdl(display->env);
	if (!fill_sdlenv(&(display->env), TITLE, HEIGHT, WIDTH))
		return (0);
	//load_music(display->env, MUSIC_PATH);
	display->font_text = TTF_OpenFont(FONT_TYPE_TXT, FONT_SIZE_TXT);
	init_vars_display(display);
	return (1);
}
