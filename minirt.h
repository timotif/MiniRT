/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:43:46 by tfregni           #+#    #+#             */
/*   Updated: 2023/06/02 11:59:17 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include "keys.h"
# define SPACE "\t "
# include <mlx.h>
# define SPACE "\t\n\f\r\v "

typedef enum e_err
{
	SUCCESS,
	ARG_REQUIRED,
	FILE_EXTENSION,
	INVALID_FILE,
	INVALID_ELEMENT,
	INVALID_DUP,
}			t_err;

enum e_unique_el
{
	AMBIENT,
	CAMERA,
	LIGHT,
};

typedef struct s_img
{
	void		*mlx_ptr;
	void		*win_ptr;
	char		*name;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
}				t_img;

typedef struct s_point
{
	float	x;
	float	y;
	float	z;
}			t_point;

typedef t_point	t_vector;

typedef struct s_ambient
{
	float	lighting_ratio;
	int		trgb;
}				t_ambient;

typedef struct s_camera
{
	t_point		pos;
	t_vector	orientation;
	uint8_t		fov;
}				t_camera;

typedef struct s_light
{
	t_point		pos;
	float		brightness;
	int			trgb;
}				t_light;

typedef struct s_sphere
{
	t_point		pos;
	float		diameter;
	int			trgb;
}				t_sphere;

typedef struct s_plane
{
	t_point		pos;
	t_vector	rotation;
	int			trgb;
}				t_plane;

typedef struct s_cylinder
{
	t_point		center;
	t_vector	rotation;
	float		diameter;
	float		height;
	int			trgb;
}				t_cylinder;

/**
 * Possible struct for the scene:
 * one value for each unique element (A, C, L),
 * an array for each solid. Plus a pointer
 * to the image.
*/
typedef struct s_scene
{
	t_img		*img;
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_sphere	*sp;
	t_plane		*pl;
	t_cylinder	*cy;
}				t_scene;

/* PARSE */
int		parse_args(t_scene *scene, char *filename);
t_err	validate_ambient(t_scene *scene, char **el);
t_err	validate_camera(t_scene *scene, char **el);
t_err	validate_light(t_scene *scene, char **el);

/* PARSE UTILS */
int		validate_3d_range(t_point point, float min, float max);
t_point	extract_xyz(char *xyz);
int		extract_rgb(char *rgb);

/* GRAPHIC */
int		create_trgb(int t, int r, int g, int b);

/* ERROR HANDLING*/
t_err	ft_error(char *msg, char *arg, int err_code);

#endif
