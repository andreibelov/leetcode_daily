# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    .gdbinit                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/05 21:32:04 by abelov            #+#    #+#              #
#    Updated: 2025/09/05 21:32:04 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

set width 0
set print elements 0
set print pretty off
set print array off
display *((int (*)[3]) *board) @ boardSize
display *(int(*)[3])counts @ 4
disp it
disp view
disp pos
disp neighbour
disp alive_neighbours
disp count