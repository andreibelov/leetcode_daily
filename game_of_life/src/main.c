/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 02:41:43 by abelov            #+#    #+#             */
/*   Updated: 2025/09/02 02:41:43 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <sys/param.h>
#include "leetcode_daily.h"

typedef struct s_point {
	int x,y;
}	Point;

#if USING_ASAN
static inline __attribute__((always_inline))
Point add_impl(Point a, Point b)
{ return (Point){ .x = a.x + b.x, .y = a.y + b.y}; }
#endif

/**
 * 289. Game of Life
 *
 * @param board
 * @param boardSize
 * @param boardColSize
 */
void gameOfLife(int **board, int boardSize, const int *boardColSize)
{
	Point it;
	Point lim = { .x = boardColSize[0], .y = boardSize };

	enum e_dir { N = 0, NE, E, SE, S, SW, W, NW, DIR_MAX };
	static const Point views[DIR_MAX] = {
		[NW] = {-1, -1}, [N] = {0, -1}, [NE] = {1, -1},
		[W]  = {-1,  0},				[E]  = {1,  0},
		[SW] = {-1,  1}, [S] = {0,  1}, [SE] = {1,  1},
	};

	int *counts = (int *)calloc((size_t) lim.x * lim.y, sizeof(int));
	if (!counts) {
		fprintf(stderr, "gameOfLife: calloc failed: %m\n");
		return;
	}

	Point	(*add)(Point, Point);
#if !USING_ASAN && !defined(__clang__)
	auto Point (*add_impl)(Point, Point);
	add = ({ Point add_impl(Point a, Point b)
	{ return (Point){ a.x + b.x, a.y + b.y}; } add_impl; });
#else
	add = add_impl;
#endif

	it.y = -1;
	while (++it.y < lim.y) {
		int *counts_row = &counts[((size_t)it.y * lim.x)];
		it.x = -1;
		while (++it.x < lim.x) {
			int acc = 0;
			int k = 0;
			while (k < DIR_MAX) {
				int dir = (E + k++) % DIR_MAX;
				Point pos = add(it, views[dir]);
				if (pos.x >= 0 && pos.x < lim.x && pos.y >= 0 && pos.y < lim.y)
				{
					int *board_row = board[pos.y];
					acc += board_row[pos.x];
				}
			}
			counts_row[it.x] = acc;
		}
	}
	it.y = -1;
	while (++it.y < lim.y) {
		int *board_row = board[it.y];
		int *counts_row = &counts[((size_t)it.y * lim.x)];
		it.x = -1;
		while (++it.x < lim.x) {
			int cell = board_row[it.x];
			int cnt  = counts_row[it.x];
			board_row[it.x] = ((cell && (cnt > 1 && cnt < 4)) || (!cell && cnt == 3));
		}
	}
	free(counts);
}

void gameOfLife2(int **board, int boardSize, const int *boardColSize)
{
	Point it;
	Point lim = { .x = boardColSize[0], .y = boardSize };

	enum e_slot { CURR = 0, NEXT, SLOTS };
	enum e_dir { N = 0, NE, E, SE, S, SW, W, NW, DIR_MAX };
	static const int dx[DIR_MAX] = {
		[NW] = -1, [N] =  0, [NE] =  1,
		[W]  = -1, 			 [E]  =  1,
		[SW] = -1, [S] =  0, [SE] =  1,
	};
	static const int dy[DIR_MAX] = {
		[NW] = -1, [N] = -1, [NE] = -1,
		[W]  =  0, 			 [E]  =  0,
		[SW] =  1, [S] =  1, [SE] =  1,
	};

	int *counts = (int *)calloc((size_t) lim.x * SLOTS, sizeof(int));
	if (!counts)
		return (void) (fprintf(stderr, "gameOfLife: alloc failed: %m\n"));

	int *brd_rows[SLOTS] = { NULL, NULL };
	int *cnt_rows[SLOTS] = { counts, counts + lim.x };

	it.y = -1;
	while (++it.y < lim.y) {
		/* parity slot trick */
		size_t curr = it.y & 1;
		size_t prev = curr ^ 1;

		int next_row = it.y + 1;
		const int has_next_row = (next_row < lim.y);
		brd_rows[CURR] = board[it.y];
		brd_rows[NEXT] = (has_next_row) ? board[next_row] : NULL;

		cnt_rows[CURR] = &counts[curr * lim.x];
		cnt_rows[NEXT] = &counts[prev * lim.x];

		it.x = -1;
		while (++it.x < lim.x) {
			int			acc = cnt_rows[CURR][it.x];
			const int	cell = brd_rows[CURR][it.x];

			/* read-ahead neighbors and push forward contributions */
			enum e_dir dir = NE;
			while (++dir < W) {
				Point pos = {.x = it.x + dx[dir], .y = dy[dir]};
				_Bool oob = pos.x < 0 || pos.x >= lim.x || pos.y < 0 ||
						   pos.y >= SLOTS || !brd_rows[pos.y];
				if (oob) /* skip OOB and keep going */
					continue ;
				cnt_rows[pos.y][pos.x] += cell;
				acc += brd_rows[pos.y][pos.x];
			}
			brd_rows[CURR][it.x] = (cell ? (acc == 2 || acc == 3) : (acc == 3));
		}
		memset(cnt_rows[CURR], 0, (size_t)lim.x * sizeof(int));
	}
	free(counts);
}

static inline
int next(int *iter, Point center, Point *out, Point lim)
{
	enum e_dir { N = 0, NE, E, SE, S, SW, W, NW, DIR_MAX };

	static const int dx[DIR_MAX] = {
		[NW] = -1, [N] =  0, [NE] =  1,
		[W]  = -1, 			 [E]  =  1,
		[SW] = -1, [S] =  0, [SE] =  1,
	};
	static const int dy[DIR_MAX] = {
		[NW] = -1, [N] = -1, [NE] = -1,
		[W]  =  0, 			 [E]  =  0,
		[SW] =  1, [S] =  1, [SE] =  1,
	};

	while (*iter < DIR_MAX) {
		int dir = (E + (*iter)++) & (DIR_MAX - 1); /* wrap 0..7 */
		Point pos = {.x = center.x + dx[dir], .y = center.y + dy[dir]};
		if (pos.x < 0 || pos.x >= lim.x || pos.y < 0 || pos.y >= lim.y)
			continue; /* skip OOB and keep going */
		*out = pos;
		return 1;
	}
	return 0;
}

void gameOfLife3(int **board, int boardSize, const int *boardColSize)
{
	Point lim = {.x = boardColSize[0], .y = boardSize };

	int (*counts)[lim.x] = calloc((size_t)lim.y * lim.x, sizeof(int));

	Point it;

	it.y = -1;

	while (++it.y < lim.y)
	{
		int *counts_row = counts[it.y];
		it.x = -1;
		while (++it.x < lim.x)
		{
			Point pos;
			int acc = 0;
			int iter = 0;
			while (next(&iter, it, &pos, lim))
				acc += board[pos.y][pos.x];

			int count = counts_row[it.x];
			count += acc;
			counts_row[it.x] = count;
		}
	}
	it.y = -1;
	while (++it.y < lim.y)
	{
		int *board_row = board[it.y];
		int *counts_row = counts[it.y];

		it.x = -1;
		while (++it.x < lim.x)
		{
			int cell = board_row[it.x];
			int cnt = counts_row[it.x];
			board_row[it.x] = (cell && (cnt > 1 && cnt < 4)) || (!cell && cnt == 3);
		}
	}
	free(counts);
}

typedef struct s_input
{
	Grid grid;
	Grid expected;
}	Case;

int ft_do_test(Case *input)
{
	int check_val;

	Grid board = input->grid;
	Grid expected = input->expected;

	int **rows = (int **) malloc((size_t) board.rows * sizeof(int *));
	if (rows == NULL)
		return -1;
	int r = -1;
	while (++r < board.rows)
		rows[r] = board.a + (size_t)r * board.cols;
	gameOfLife2(rows, board.rows, &board.cols);

	check_val = ((expected.cols == board.cols) && (expected.rows == board.rows));
	if (!check_val)
	{
		int result_size = board.cols * board.rows;
		int expected_size = expected.cols * expected.rows;

		printf("resultTotalSize doesn't match expected value\n");
		printf("got array of size \"%d\" whilst \"%d\" was to be expected\n",
			   result_size, expected_size);
	}
	else
	{
		check_val = !grid_cmp(&board, &expected);
		if (!check_val)
		{
			printf("got \n");
			grid_print(&board);
			printf(" whilst \n");
			grid_print(&expected);
			printf(" was to be expected \n");
		}
	}
	check(check_val);
	free((void *) rows);
	return (0);
}

int main(void)
{
	int i;
	Case cases[] = {
		{
			.grid = {.a = (int[]) {
				0, 1, 0,
				0, 0, 1,
				1, 1, 1,
				0, 0, 0
			}, .cols = 3, .rows = 4},
			.expected = {.a = (int[]) {
				0, 0, 0,
				1, 0, 1,
				0, 1, 1,
				0, 1, 0
			}, .cols = 3, .rows = 4},
		},
		{
			.grid = {.a = (int[]) {
				1, 1,
				1, 0
			}, .cols = 2, .rows = 2},
			.expected = {.a = (int[]) {
				1, 1,
				1, 1
			}, .cols = 2, .rows = 2},
		},
		{
			.grid = {.a = (int[]) {
				0
			}, .cols = 1, .rows = 1},
			.expected = {.a = (int[]) {
				0
			}, .cols = 1, .rows = 1},
		},
	};
	int cases_size = (int) (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, segv_handler);
	i = -1;
	while (++i < cases_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
