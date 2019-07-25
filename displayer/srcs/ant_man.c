/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ant_man.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 09:09:54 by obelouch          #+#    #+#             */
/*   Updated: 2019/07/25 10:37:31 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "displayer.h"

static void	fill_c(t_point *c, t_point o, int r)
{
	c[0] = pt_new(o.y, o.x - 3 * r);
	c[1] = pt_new(o.y, o.x - r);
	c[2] = pt_new(o.y, o.x + r);
	c[3] = pt_new(o.y, o.x + 3 * r);
}

static void	fill_rot_c(t_point *rot_c, t_point *c, t_point o, int teta)
{
	rot_c[0] = pt_rot(o, c[0], teta);
	rot_c[1] = pt_rot(o, c[1], teta);
	rot_c[2] = pt_rot(o, c[2], teta);
	rot_c[3] = pt_rot(o, c[3], teta);
}

void		ant_man(t_display *display, SDL_Color color, t_point o, int teta)
{
	t_point	c[4];
	t_point	rot_c[4];
	t_point	p[2];
	int		r;
	int		i;

	r = (display->block / 15) / 3;
	fill_c(c, o, r);
	fill_rot_c(rot_c, c, o, teta);
	p[0] = pt_rot(o, pt_new(c[0].y + 3 * r / 2, c[0].x - 3 * r / 2), teta);
	p[1] = pt_rot(o, pt_new(c[0].y - 3 * r / 2, c[0].x - 3 * r / 2), teta);
	sdl_line(display->env, sdl_rgb(0, 0, 0), rot_c[0], p[0]);
	sdl_line(display->env, sdl_rgb(0, 0, 0), rot_c[0], p[1]);
	sdl_disk(display->env, color, rot_c[0], r);
	sdl_circle(display->env, sdl_rgb(0, 0, 0), rot_c[0], r);
	i = 1;
	while (i < 4)
	{
		p[0] = pt_rot(o, pt_new(c[i].y - 2 * r, c[i].x), teta);
		p[1] = pt_rot(o, pt_new(c[i].y + 2 * r, c[i].x), teta);
		sdl_line(display->env, sdl_rgb(0, 0, 0), p[0], p[1]);
		sdl_disk(display->env, color, rot_c[i], r);
		sdl_circle(display->env, sdl_rgb(0, 0, 0), rot_c[i], r);
		i++;
	}
}
