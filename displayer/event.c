/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 02:08:59 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/24 02:12:29 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

static void		event_zoom(t_display *display)
{
	if (display->event.key.keysym.sym == SDLK_KP_PLUS &&
			display->block < WIDTH / 5)
	{
		display->block += 10;
		display->offset.x -= 20;
	}
	if (display->event.key.keysym.sym == SDLK_KP_MINUS &&
			display->block > WIDTH / 50)
	{
		display->block -= 10;
		display->offset.x += 20;
	}
}

static void		event_keydown(t_display *display)
{
	if (display->event.key.keysym.sym == SDLK_ESCAPE)
		display->pass = 0;
	else if (display->event.key.keysym.sym == SDLK_SPACE)
		display->pause = (display->pause) ? 0 : 1;
	else if (display->event.key.keysym.sym == SDLK_RIGHT)
		display->offset.x += 20;
	else if (display->event.key.keysym.sym == SDLK_LEFT)
		display->offset.x -= 20;
	else if (display->event.key.keysym.sym == SDLK_UP)
		display->offset.y -= 20;
	else if (display->event.key.keysym.sym == SDLK_DOWN)
		display->offset.y += 20;
	else if (display->event.key.keysym.sym == SDLK_r)
		init_vars_display(display);
	else if (display->event.key.keysym.sym == SDLK_b)
	{
		display->moment -= (display->moment > 0) ? 1 : 0;
		draw_state(display, display->infos);
	}
	else if (display->event.key.keysym.sym == SDLK_f)
	{
		display->moment += (display->moment < display->infos.shots) ? 1 : 0;
		draw_state(display, display->infos);
	}
	else if (display->event.key.keysym.sym == SDLK_KP_PLUS ||
			display->event.key.keysym.sym == SDLK_KP_MINUS)
		event_zoom(display);
}

void			displayer_loop(t_display *display)
{
	while (display->pass)
	{
		while (SDL_PollEvent(&(display->event)))
		{
			if (display->event.type == SDL_QUIT)
				display->pass = 0;
			if (display->event.type == SDL_KEYDOWN)
				event_keydown(display);
		}
		if (!display->pause)
		{
			if (display->moment < display->infos.shots)
				display->moment++;
			else
				display->pause = 1;
			SDL_Delay(200);
		}
		draw_state(display, display->infos);
	}
}
