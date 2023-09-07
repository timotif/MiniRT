/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 20:41:01 by tfregni           #+#    #+#             */
/*   Updated: 2023/09/07 14:44:47 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ray.h"
#include "matrix_math.h"

#ifdef DEBUG

void	print_4x4(t_matrix_trans m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f\t", m[i][j]);
		printf("\n");
	}
	printf("\n");
}
#endif

/**
 * As in openGL, we state that the projection plane coinincides
 * with the near clipping plane.
 * https://www.scratchapixel.com/lessons/3d-basic-rendering
 * /perspective-and-orthographic-projection-matrix
 * /opengl-perspective-projection-matrix.html
*/
t_point_3d	world_to_cam(t_point_3d p, t_matrix_trans proj)
{
	return (transform(p, proj));
}

t_point_3d	world_to_cam_new(t_point_3d p, t_matrix transform)
{
	return (mx_mult(transform, p));
}

int	apply_lighting_ratio(int trgb, float lighting_ratio)
{
	return (create_trgb(0, \
			lighting_ratio * ((trgb >> 16) & 0xFF),
			lighting_ratio * ((trgb >> 8) & 0xFF),
			lighting_ratio * (trgb & 0xFF)));
}

t_color	apply_light_to_color(t_color c, double light)
{
	t_color	ret;

	ret = color(0.0, c.r * light, c.g * light, c.b * light);
	return (ret);
}

/**
 * @brief converts an int in trgb format to a color struct with double values
 * in range [0, 1]
 * @param trgb int in trgb format
 * @return color struct
*/
t_color	convert_color(int trgb)
{
	t_color	c;

	c.t = ((trgb >> 24) & 0xFF) / 255.999;
	c.r = ((trgb >> 16) & 0xFF) / 255.999;
	c.g = ((trgb >> 8) & 0xFF) / 255.999;
	c.b = (trgb & 0xFF) / 255.999;
	return (c);
}

/**
 * @brief converts a color struct with double values in range [0, 1]
 * to an int in trgb format
 * @param c color struct
 * @return int in trgb format
*/
int	convert_trgb(t_color c)
{
	int		t;
	int		r;
	int		g;
	int		b;

	t = (int)(c.t * 255.999);
	r = (int)(c.r * 255.999);
	g = (int)(c.g * 255.999);
	b = (int)(c.b * 255.999);
	return (t << 24 | r << 16 | g << 8 | b);
}

/**
 * @brief creates a ray from the camera to the canvas
 * 1. Pixel to world
 * 2. Transform through matrix
 * 3. Get ray direction
 * 4. Normalize ray direction
*/

t_ray	create_cam_ray(t_camera *c, double int_x, double int_y)
{
	t_ray	ray;

	ray.origin = c->pos;
	t_vec3 pxl_x = vec3_mult(c->pxl_size_hor, int_x);
	t_vec3 pxl_y = vec3_mult(c->pxl_size_ver, int_y);
	t_vec3 pxl_pos = vec3_sum(c->viewport_top_left, vec3_mult(c->right, vec3_len(pxl_x)));
	pxl_pos = vec3_sum(pxl_pos, vec3_mult(vec3_inv(c->up), vec3_len(pxl_y)));
	ray.direction = vec3_sub(pxl_pos, ray.origin);
	ray.direction = vec3_unit(ray.direction);
	return (ray);
}

/**
 * Former intersect_element function
 * @brief Checks if the ray hits any element in the scene
 * In the scene->shape[i].hit(&scene->shape[i], ray, rec) call
 * the ray is updated with hit distance, hit point, normal and color
*/
bool	hit_element(t_scene *scene, t_ray ray, t_hit_record *rec)
{
	bool	hit_anything;
	int		i;

	hit_anything = false;
	rec->t = RAY_LEN;
	i = scene->shape_count - 1;
	while (i >= 0)
	{
		if (scene->shape[i].hit(&scene->shape[i], ray, rec) && rec->t > EPSILON)
		{
			hit_anything = true;
		}
		i--;
	}
	return (hit_anything);
}

/**
 * @brief Creates a hit record that gets updated if the ray hits an element
 * @returns the color of the hit element
*/
t_color	ray_color(t_scene *scene, t_ray r)
{
	double			light;
	t_hit_record	rec;

	if (hit_element(scene, r, &rec))
	{
		light = ft_fmin(light_coeff(scene, &rec), 1);
		if (light < 0 || light > 1)
			printf("Light out of bound %f\n", light); // remember to remove
		return (apply_light_to_color(rec.color, light));
	}
	return (convert_color(scene->ambient.trgb));
}


// /**
//  * @brief Returns a hit record for a ray that doesn't hit anything
// */
// t_hit_record	miss(t_ray r)
// {

// };


// /**
//  * @brief Returns the closest hit record
//  * @param r: ray
//  * @param t: hit distance
//  * @param i: index of the shape
// */
// t_hit_record	closest_hit(t_ray r, double t, uint32_t i)
// {

// };

// /**
//  * @brief Returns a hit point payload
//  * @param r: ray
// */
// t_hit_record	trace_ray(t_scene *s, t_ray r)
// {
// 	t_hit_record	rec;

// 	if (hit_element(s, r, &rec))
// 	{

// 	}
// };

/**
 * @returns a color as int
 * @math:
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
 * https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
*/
int	per_pixel(t_pxl p, t_scene *scene)
{
	t_ray	r;
	t_color	c;

	r = create_cam_ray(&scene->camera, p.x, p.y);
	c = ray_color(scene, r);
	return (convert_trgb(c));
}

/**
 * The trgb value of a pxl not intersecting a solid is each 8bit color
 * multiplied by the lighting ratio
*/
void	draw(t_scene *scene)
{
	t_pxl	p;
	t_img	*data;
	int		c;

	data = scene->img;
	cam_setup(&scene->camera);
	p.y = 0;
	while (p.y < HEIGHT)
	{
		// printf("Scanlines remaining: %d\n", HEIGHT - 1 - p.y);
		p.x = 0;
		while (p.x < WIDTH)
		{
			c = per_pixel(p, scene);
			my_mlx_pixel_put_d(data, p.x, p.y, c);
			p.x++;
		}
		p.y++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
}

t_err	render_scene(t_scene *scene)
{
	if (!init_img(scene))
	{
		draw(scene);
		mlx_manage(scene);
		return (SUCCESS);
	}
	return (ft_error("render", "couldn't init image", MEM_FAIL, scene));
}
