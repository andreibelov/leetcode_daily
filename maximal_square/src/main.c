/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:33:26 by abelov            #+#    #+#             */
/*   Updated: 2025/09/07 21:33:26 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <sys/param.h>

#include "leetcode_daily.h"

/**
 * 221. Maximal Square
 *
 * Given an m x n binary matrix filled with 0's and 1's,
 * find the largest square containing only 1's and return its area.
 * @param matrix
 * @param matrixSize
 * @param matrixColSize
 * @return area of the biggest possible square
 */
int maximalSquare(char** matrix, int matrixSize, int* matrixColSize) {
	typedef struct s_point { int x,y; }	Point;
	if (matrixSize == 0 || matrixColSize == NULL || matrixColSize[0] == 0)
		return 0;

	Point p;
	int rows = matrixSize;
	int cols = matrixColSize[0];
	int maxSideLength = 0;

	size_t h = (size_t)rows + 1;

	// when changing pointer type, sizeof(*ptr) changes automatically to match.
	// it is self-maintaining:
	// 		if change T or p’s type, the allocation stays correct.
	int (*dp)[(size_t)cols + 1] = calloc(h, sizeof(*dp));
//	int (*dp)[cols + 1] = calloc(((size_t) cols + 1) * (rows + 1), sizeof(int));

	if (!dp)
		return 0;

	p.y = -1;
	while (++p.y < rows) {
		p.x = -1;
		while (++p.x < cols) {
			if (matrix[p.y][p.x] == '1')
			{
				int minNeighbor = MIN(dp[p.y][p.x],
									MIN(dp[p.y + 1][p.x], dp[p.y][p.x + 1]));
				dp[p.y + 1][p.x + 1] = minNeighbor + 1;
				maxSideLength = MAX(maxSideLength, dp[p.y + 1][p.x + 1]);
			}
			else
				dp[p.y + 1][p.x + 1] = 0;
		}
	}
//	ft_print_int_tab((int *) dp, ((size_t) cols + 1) * (rows + 1), "");
	free(dp);
	return maxSideLength * maxSideLength;
}

int maximalSquare2(char** matrix, int matrixSize, int* matrixColSize) {
	if (matrixSize == 0 || matrixColSize == NULL || matrixColSize[0] == 0)
		return 0;
	int rows = matrixSize;
	int cols = matrixColSize[0];
	int maxSideLength = 0;

	int (*dp)[(size_t) cols + 1] = calloc(2, sizeof(*dp));
	if (!dp)
		return 0;

	struct s_point { int x,y; } p;
	p.y = -1;
	while (++p.y < rows) {
		size_t prev = p.y & 1;
		size_t curr = prev ^ 1;

		dp[curr][0] = 0;  /* sentinel */
		p.x = 1; // dp[0] is always 0 - sentinel at index 0
		while (p.x <= cols) {
			int upper = dp[prev][p.x];    	// dp[p.y-1][p.x]
			int left  = dp[curr][p.x - 1];	// dp[p.y  ][p.x-1]
			int diag  = dp[prev][p.x - 1];	// dp[p.y-1][p.x-1]

			if (matrix[p.y][p.x - 1] == '1')
			{
				int minNeighbor = MIN(diag, MIN(upper, left));
				dp[curr][p.x] = minNeighbor + 1;
				maxSideLength = MAX(maxSideLength, minNeighbor + 1);
			}
			else
				dp[curr][p.x] = 0;
			p.x++;
		}
	}
	free(dp);
	return maxSideLength * maxSideLength;
}

int maximalSquare3(char** matrix, int matrixSize, int* matrixColSize) {
	if (matrixSize == 0 || matrixColSize == NULL || matrixColSize[0] == 0)
		return 0;

	int rows = matrixSize;
	int cols = matrixColSize[0];
	int maxSideLength = 0;

	size_t w = (size_t)cols + 1;
	int *dp_row = calloc(w, sizeof(int));
	if (!dp_row)
		return 0;

	struct s_point { int x,y; } p;
	p.y = -1;
	while (++p.y < rows) {
		int prev_diag = 0;	// dp[i-1][0]
		p.x = 1; // dp_row[0] is always 0 - sentinel at index 0
		while (p.x <= cols) {
			int upper = dp_row[p.x];    	// dp[p.y-1][p.x]
			int left  = dp_row[p.x - 1];	// dp[p.y  ][p.x-1]
			int diag  = prev_diag;			// dp[p.y-1][p.x-1]

			if (matrix[p.y][p.x - 1] == '1')
			{
				int minNeighbor = MIN(diag, MIN(upper, left));
				dp_row[p.x] = minNeighbor + 1;
				maxSideLength = MAX(maxSideLength, minNeighbor + 1);
			}
			else
				dp_row[p.x] = 0;
			prev_diag = upper;
			p.x++;
		}
	}
	free(dp_row);
	return maxSideLength * maxSideLength;
}

typedef struct s_input
{
	CharGrid grid;
	int expected;
}	Case;

int ft_do_test(Case *input)
{
	int check_val;

	CharGrid matrix = input->grid;
	int result;
	int expected = input->expected;

	char **rows = (char **) malloc((size_t) matrix.rows * sizeof(char *));
	if (rows == NULL)
		return -1;
	int r = -1;
	while (++r < matrix.rows)
		rows[r] = matrix.a + (size_t)r * matrix.cols;
	result = maximalSquare2(rows, matrix.rows, &matrix.cols);

	check_val = (result == expected);
	if (!check_val)
		printf("got \"%d\" whilst \"%d\" was to be expected\n",
			   result, expected);
	check(check_val);
	free((void *) rows);
	return (0);
}

int main(void)
{
	int i;
	Case cases[] = {
		{
			.grid = {.a = (char[]) {
				'1','0','1','0','0',
				'1','0','1','1','1',
				'1','1','1','1','1',
				'1','0','0','1','0'
			}, .cols = 5, .rows = 4},
			.expected = 4,
		},
		{
			.grid = {.a = (char[]) {
				'0','1',
				'1','0',
			}, .cols = 2, .rows = 2},
			.expected = 1,
		},
		{
			.grid = {.a = (char[]) {
				'0'
			}, .cols = 1, .rows = 1},
			.expected = 0,
		},
		{
			.grid = {.a = (char[]) {
				'1'
			}, .cols = 1, .rows = 1},
			.expected = 1,
		},
	};
	int cases_size = (int) (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, segv_handler);
	i = -1;
	while (++i < cases_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
