/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 22:59:00 by abelov            #+#    #+#             */
/*   Updated: 2025/02/23 22:59:01 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <alloca.h>
#include <stdlib.h>

/**
 * 119. Pascal's Triangle II
 *
 * Given an integer rowIndex, return the rowIndexth (0-indexed) row
 * of the Pascal's triangle. In Pascal's triangle, each number is the sum
 * of the two numbers directly above
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* getRow(int rowIndex, int* returnSize)
{
	if (rowIndex < 0)
	{
		returnSize = 0;
		return (NULL);
	}
	int *dp = (int *)calloc(rowIndex + 1,  sizeof(int));
	dp[0] = 1;

	for (int i = 1; i < rowIndex + 1; i++)
	{
		for (int j = i; j > 0; j--)
			dp[j] += dp[j - 1];
	}
	*returnSize = rowIndex + 1;
	return (dp);

}
