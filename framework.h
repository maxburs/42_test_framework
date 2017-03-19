/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framework.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburson <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/14 21:26:52 by mburson           #+#    #+#             */
/*   Updated: 2017/02/14 21:27:31 by mburson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAMEWORK_H
# define FRAMEWORK_H

# include <string.h>

# define RESET write(1, "\x1b[0m", 4)
# define GREEN write(1, "\x1b[32m", 5)
# define RED write(1, "\x1b[31m", 5)
# define INVERSE write(1, "\x1b[7m", 4)
# define BOLD write(1, "\x1b[1m", 4);

# define BUFF_SIZE 100

void				test(_Bool last, char const *name,
									int (*test)(void), char *output);

#endif
