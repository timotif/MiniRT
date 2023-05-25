/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfregni <tfregni@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 12:22:28 by tfregni           #+#    #+#             */
/*   Updated: 2023/02/19 13:03:12 by tfregni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H
/*
Keymap
*/
# define UP	65362
# define DOWN 65364
# define LEFT 65361
# define RIGHT 65363
# define ESC 65307
# define C 99
# define R 114
/*
Mouse map
*/
# define LEFT_BUTTON 1
# define RIGHT_BUTTON 3
# define CENTER_BUTTON 2
# define WHEEL_UP 4
# define WHEEL_DOWN 5
/*
Events
*/
enum {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

#endif
