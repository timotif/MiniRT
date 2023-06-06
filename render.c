/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 20:41:01 by tfregni           #+#    #+#             */
/*   Updated: 2023/06/06 16:09:06 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "vector_math.h"

float	ft_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float	ft_min(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

/**
 * range_x / WIDTH = canvas size of 1 pxl on x axis
 * range_x / WIDTH * ratio = canvas size of 1 pxl on y axis
 * added - to flip the y axis
*/
t_point_2d	to_canvas(t_pxl pxl)
{
	t_point_2d	ret;
	float		range_x;
	float		ratio;

	range_x = CANV_MAX_X - CANV_MIN_X;
	ratio = WIDTH / HEIGHT;
	ret.x = -(range_x / WIDTH * pxl.x + CANV_MIN_X);
	ret.y = -(range_x / WIDTH * ratio * pxl.y + CANV_MIN_Y);
	return (ret);
}

int	apply_ligthing_ratio(int trgb, float lighting_ratio)
{
	return (create_trgb(0, \
			lighting_ratio * ((trgb >> 16) & 0xFF),
			lighting_ratio * ((trgb >> 8) & 0xFF),
			lighting_ratio * (trgb & 0xFF)));
}

float	calc_hit_point(float discriminant, float a, float b)
{
	return ((-b - sqrt(discriminant)) / (2.0f * a));
}

/**
 * It returns the discriminant. If it's >=0 it assigns to t the value of
 * the closest hit point
*/
float	intersect_sphere(t_scene *scene, t_vector ray_direction, float *t)
{
	float			a;
	float			b;
	float			c;
	t_point_3d		transl;
	float			discriminant;

	a = vect_dot(ray_direction, ray_direction);
	transl = vect_sub(scene->camera.pos, scene->sp[0].pos);
	b = 2.0f * vect_dot(transl, ray_direction);
	c = vect_dot(transl, transl) - pow((scene->sp->diameter / 2), 2);
	discriminant = b * b - (4.0f * a * c);
	if (discriminant >= 0)
		*t = calc_hit_point(discriminant, a, b);
	return (discriminant);
}

float	light_coeff(t_scene *scene, float t, t_vector ray_direction)
{
	t_vector	hit_pos;
	t_vector	normal;
	float		light;

	hit_pos = vect_sum(scene->camera.pos, vect_mult(ray_direction, t));
	normal = vect_norm(vect_sub(hit_pos, scene->sp[0].pos));
	light = ft_max(vect_dot(normal, vect_inverse((vect_norm(scene->light.pos)))), 0.0f);
	return (light);
}

/**
 * @returns a color as int
 * @math
 * circle = (x^2 - a) + (y^2 - b) - r^2 = 0 (a and b are the coord)
 * ray = a + bt (a: origin b: direction t: distance)
 * a : ray origin (scene->camera.pos)
 * b : ray direction (scene->camera.orientation)
 * r : radius (scene->sphere.diameter / 2)
 * t : hit distance
 * Substitute ray into circle and solve for t
 * (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t
 * + (ax^2 +ay^2 + az^2 - r^2) = 0
 * Quadratic formula: (-b +- sqrt(b^2 - 4ac))/2a
 * Discriminant: b^2 - 4ac
*/
int	per_pixel(t_pxl p, t_scene *scene)
{
	t_point_2d	coord;
	t_vector	ray_direction;
	float		discriminant;
	float		t;

	coord = to_canvas(p);
	ray_direction = vect_norm((t_vector){coord.x, coord.y, -1.0f});
	discriminant = intersect_sphere(scene, ray_direction, &t);
	if (discriminant >= 0)
	{
		return (apply_ligthing_ratio(scene->sp[0].trgb, light_coeff(scene, t, ray_direction)));
	}
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
