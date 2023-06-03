/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:49:11 by tvasilev          #+#    #+#             */
/*   Updated: 2023/06/03 16:56:49 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_MATH_H
# define VECTOR_MATH_H

# include "minirt.h"

t_vector	vect_sum(t_vector a, t_vector b);
t_vector	vect_sub(t_vector a, t_vector b);
float		vect_dot(t_vector a, t_vector b);
t_vector	vect_norm(t_vector v);
float		vect_mag(t_vector v);
t_vector	vect_mult(t_vector v, float a);

#endif
