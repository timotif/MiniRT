/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 20:41:01 by tfregni           #+#    #+#             */
/*   Updated: 2023/06/05 11:00:27 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector_math.h"

/**
 * Can check interception with just the discriminant of the quadratic formula
 * b^2 - 4ac. If < 0 no intersection, 0 tan, > 0 2 intersections
 * a_quad = (bx^2 + by^2) = camera.direction sum of squares
 * b_quad = (2axbx + 2ayby) = (2 * camera.origin.x * camera.direction.x)
 * c_quad = (ax^2 + bx^2 - r^2) = camera.origin^2 etc.
*/
// int	intersect_circle(t_sphere sp, t_pxl p)
// {

// }

t_point_2d	to_canvas(t_pxl pxl)
{
	t_point_2d	ret;
	float		range;
	float		ratio;

	range = CANV_MAX_X - CANV_MIN_X;
	ratio = WIDTH / HEIGHT;
	// printf("range: %f ratio: %f\n", range, ratio);
	ret.x = range * pxl.x / WIDTH + CANV_MIN_X;
	ret.y = range * ratio * pxl.y / HEIGHT + CANV_MIN_Y;
	return (ret);
}

int	apply_ligthing_ratio(int trgb, float lighting_ratio)
{
	return (create_trgb(0, \
			lighting_ratio * ((trgb >> 16) & 0xFF),
			lighting_ratio * ((trgb >> 8) & 0xFF),
			lighting_ratio * (trgb & 0xFF)));
}

/**
 * @returns a color as int
 * @math
 * circle = (x^2 - a) + (y^2 - b) - r^2 = 0 (a and b are the coord)
 * ray = a + bt (a: origin b: direction t: distance)
 * Substitute ray into circle and solve for t
 * (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 +ay^2 + az^2 - r^2) = 0
 * a : ray origin (scene->camera.pos)
 * b : ray direction (scene->camera.orientation)
 * r : radius (scene->sphere.diameter / 2)
 * t : hit distance
 * Quadratic formula: (-b +- sqrt(b^2 - 4ac))/2a
 * Discriminant: b^2 - 4ac
*/
int	per_pixel(t_pxl p, t_scene *scene)
{
	t_point_2d	coord;
	float		a;
	float		b;
	float		c;
	t_vector	ray_direction;
	float		discriminant;

	coord = to_canvas(p);
	ray_direction = vect_norm((t_vector){coord.x, coord.y, -1.0f});
	a = vect_dot(ray_direction, ray_direction);
	b = 2.0f * vect_dot(scene->camera.pos, ray_direction);
	c = vect_dot(scene->camera.pos, scene->camera.pos) - pow((scene->sp->diameter / 2), 2);
	discriminant = b * b - (4.0f * a * c);
	if (discriminant >= 0)
		return (scene->sp->trgb);
	return (apply_ligthing_ratio(scene->ambient.trgb, \
								scene->ambient.lighting_ratio));
}

/**
 * The trgb value of a pxl not intersecting a solid is each 8bit color
 * multiplied by the lighting ratio
*/
void	draw(t_scene *scene)
{
	t_pxl	p;
	t_img	*data;

	data = scene->img;
	p.y = 0;
	while (p.y < HEIGHT)
	{
		p.x = 0;
		while (p.x < WIDTH)
		{
			my_mlx_pixel_put_d(scene->img, p.x, p.y, per_pixel(p, scene));
			p.x++;
		}
		p.y++;
	}
	// p.x = 250;
	// t_point_2d point = to_canvas(p);
	// printf("pxl x: %d y: %d canvas x: %f y: %f\n", p.x, p.y, point.x, point.y);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
}

t_err	render_scene(t_scene *scene)
{
	if (!init_img(scene))
	{
		draw(scene);
		mlx_manage(scene);
	}
	return (SUCCESS);
}
