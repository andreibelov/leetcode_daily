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

static inline
int next(int *iter, Point it, Point *out, Point lim)
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
		Point pos = {.x = it.x + dx[dir], .y = it.y + dy[dir]};
		if (pos.x < 0 || pos.x >= lim.x || pos.y < 0 || pos.y >= lim.y)
			continue; /* skip OOB and keep going */
		*out = pos;
		return 1;
	}
	return 0;
}

void gameOfLife2(int **board, int boardSize, const int *boardColSize)
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

static int grid_cmp(const Grid *a, const Grid *b)
{
	if (a->rows != b->rows)
		return (a->rows - b->rows);
	if (a->cols != b->cols)
		return (a->cols - b->cols);
	size_t i = 0;
	size_t n = (size_t) a->rows * (size_t) a->cols;
	while (i < n)
	{
		int da = a->a[i];
		int db = b->a[i];
		if (da != db)
			return (da - db);
		i++;
	}
	return 0;
}

static void grid_print(const Grid *g)
{
	int row = -1;
	while (++row < g->rows)
	{
		int *grid_row = &g->a[(size_t)row * g->cols];

		int coll = -1;
		printf("[");
		while (++coll < g->cols)
		{
			printf("%d", grid_row[coll]);
			if (coll + 1 < g->cols)
				printf(" ");
		}
		printf("]\n");
	}
}

int ft_do_test(Case *input)
{
	int check_val;

//	printf("Initial:\n");
//	grid_print(&input->board);

	Grid board = input->grid;
	Grid expected = input->expected;

	int **rows = (int **) malloc((size_t) board.rows * sizeof(int *));
	if (rows == NULL)
		return -1;
	int r = -1;
	while (++r < board.rows)
		rows[r] = board.a + (size_t)r * board.cols;
	gameOfLife(rows, board.rows, &board.cols);


	check_val = ((expected.cols != board.cols) || (expected.rows != board.cols));
	if (!check_val)
	{
		int result_size = board.cols * board.cols;
		int expected_size = expected.cols * expected.cols;

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
	};
	int cases_size = (int) (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, segv_handler);
	i = -1;
	while (++i < cases_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
