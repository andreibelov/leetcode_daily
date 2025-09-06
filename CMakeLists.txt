# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/07 09:33:05 by abelov            #+#    #+#              #
#    Updated: 2024/08/27 01:55:42 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.27)


#set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER gcc-13)

include_directories(include)

project(leetcode_daily C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

set(COMMON_LDFLAGS
		-fsanitize=address,undefined,bounds,alignment,object-size
		-fsanitize=shift,signed-integer-overflow,null,return
		-fsanitize=float-divide-by-zero,float-cast-overflow
		-fno-sanitize-recover
)

set(COMMON_LIBS
		m
)

set(COMMON_CFLAGS
        -Wall -Wextra -Werror
        -Wwrite-strings
        -Wimplicit
        -Wno-error=unused-parameter
        -Wno-error=unused-variable
        -O0
        -masm=att
        -fno-exceptions
        -ffunction-sections
        -fdata-sections
        -fno-omit-frame-pointer
        -fdwarf2-cfi-asm
        -fcf-protection=none
        -mmanual-endbr
        -g3 -gdwarf-3
        -fasynchronous-unwind-tables
		-fsanitize=address,undefined,bounds,alignment,object-size
		-fsanitize=shift,signed-integer-overflow,null,return
		-fsanitize=float-divide-by-zero,float-cast-overflow
		-fno-sanitize-recover
)

add_subdirectory(two_sum)
add_subdirectory(lexicographical_numbers)
add_subdirectory(k_smallest_lexical_order)
add_subdirectory(extra_characters_in_string)
add_subdirectory(longest_common_prefix)
add_subdirectory(sum_of_prefix_scores)
add_subdirectory(my_calendar_i)
add_subdirectory(my_calendar_ii)
add_subdirectory(valid_parentheses)
add_subdirectory(remove_invalid_parentheses)
add_subdirectory(pascals_triangle)
add_subdirectory(pascals_triangle_ii)
add_subdirectory(rotate_array)
add_subdirectory(game_of_life)
