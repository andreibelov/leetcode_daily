/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 20:04:40 by abelov            #+#    #+#             */
/*   Updated: 2025/02/22 20:04:40 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

/**
 * 118. Pascal's Triangle
 *
 * Given an integer numRows, return the first numRows of Pascal's triangle.
 * In Pascal's triangle, each number is the sum of the two numbers above it
 *
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 *
 * Note: Both returned array and *columnSizes array must be malloced,
 * 	assume caller calls free().
 * @see: what's dangerous about casting the result of malloc?
 * 	https://stackoverflow.com/questions/1565496/
 */
int **generate(int numRows, int *returnSize, int **returnColumnSizes)
{
	if (numRows < 0) return 0;

	int **array = (int **)malloc(numRows * sizeof(int *));
	*returnColumnSizes = (int *)malloc(numRows * sizeof(int));

	int *dp = (int *)alloca(numRows * sizeof(int));
	*dp = 1;
	for (int i = 0; ++i < numRows; dp[i] = 0);

	*array = (int *)calloc(1, sizeof(int));
	**array = 1;
	*returnColumnSizes[0] = 1;
	for (int i = 1; i < numRows; i++)
	{
		for (int j = i; j > 0; j--)
			dp[j] += dp[j - 1];
		int last = i + 1;

		array[i] = *(int (*)[last]) malloc(1 * sizeof(int[last]));
		memmove(array[i], dp, (last) * sizeof(int));
		(*returnColumnSizes)[i] = last;
	}
	*returnSize = numRows;
	return (array);
}