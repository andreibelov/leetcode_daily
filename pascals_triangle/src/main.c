/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 20:08:34 by abelov            #+#    #+#             */
/*   Updated: 2025/02/22 20:08:35 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 118. Pascal's Triangle
 *
 * Given an integer numRows, return the first numRows of Pascal's triangle.
 * In Pascal's triangle, each number is the sum of the two numbers above it
 *
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 *
 * Constraints:
 * 	1 <= numRows <= 30
 */
int **generate(int numRows, int *returnSize, int **returnColumnSizes);

typedef struct s_test_case
{
	const int	numRows;
	TwoDArray expected;
}	Input;

int ft_do_test(struct s_test_case *testcase)
{
	int i;
	int *sizes;
	int **arrays;
	Array pool[1024];

	TwoDArray res = {0};
	int check_val;

	res.arrays = pool;

	arrays = generate(testcase->numRows, &res.size, &sizes);
	i = -1;
	while (++i < res.size)
		res.arrays[i] = (Array) {.arr = arrays[i], .size = sizes[i]};

	check_val = (res.size == testcase->expected.size);
	if (!check_val)
	{
		printf("got \"%d\" elements whilst \"%d\" was to be expected\n",
			   res.size, testcase->expected.size);
	}
	else if (res.size != 0)
	{
		i = -1;
		while(++i < res.size)
		{
			Array *p = &res.arrays[i];
			Array *q = &testcase->expected.arrays[i];
			check_val = (arraycmp(p, q) == 0);
			if (!check_val)
			{
				check(check_val);
				printf("at index [%d] got ", i);
				ft_print_array_eol(p, " whilst ");
				ft_print_array_eol(q, "");
				printf(" was to be expected \n");
				free(p->arr);
				while(++(i) < res.size)
					free(res.arrays[i].arr);
				break;
			}
			free(p->arr);
		}
	}
	if (check_val)
		check(check_val);
	free(arrays);
	free(sizes);
	return (0);
}

int main(void)
{
	int i;
	struct s_test_case cases[] = {
		{
			.numRows = 5,
			.expected = (TwoDArray){
				.arrays = (Array [5]){
					{(int [1]){1}, .size = 1},
					{(int [2]){1,1}, .size = 2},
					{(int [3]){1,2,1}, .size = 3},
					{(int [4]){1,3,3,1}, .size = 4},
					{(int [5]){1,4,6,4,1}, .size = 5}
				},
				.size = 5
			}
		},
		{
			.numRows = 1,
			.expected = (TwoDArray){
				.arrays = (Array [1]){
					{(int [1]){1}, .size = 1},
				},
				.size = 1
			}
		},
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
