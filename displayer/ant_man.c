/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ant_man.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 09:09:54 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/25 09:39:23 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

void		ant_man(t_sdlenv env, SDL_Color color, t_point origin, int g_r, int teta)
{
	t_point	c[4];
	t_point	rot_c[4];
	t_point	p[2];
	int		r;
	int		i;

	r = g_r / 3;
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
