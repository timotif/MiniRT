/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handler.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvasilev <tvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:32:50 by tvasilev          #+#    #+#             */
/*   Updated: 2023/09/16 14:36:54 by tvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HANDLER_H
# define EVENT_HANDLER_H

# include "minirt.h"

// int	close_window(t_img *data);
int		key_handle(int keycode, t_scene *scene);
int		mouse_handle(int button, int x, int y, t_scene *scene);
// int	track_delta(int button, int x, int y, t_scene *scene);

#endif
