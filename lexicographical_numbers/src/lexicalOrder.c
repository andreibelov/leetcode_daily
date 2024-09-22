/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicalOrder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 00:58:46 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 00:58:47 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/**
 * 386. Lexicographical Numbers
 *
 * Given an integer n, return all the numbers in the range [1, n]
 * sorted in lexicographical order.
 */
int *lexicalOrder(int n, int *returnSize);

#if defined USE_RECURSION

void DFS(int *s, int *size, int n, int last)
{
	int i;
	int x;

	i = -1;
	while (++i <= 9)
	{
		if (last == 0 && i == 0)
			i = 1;
		x = i + last;
		if (x > n)
			return;
		s[(*size)++] = x;
		if (x * 10 <= n)
			DFS(s, size, n, x * 10);
	}
}

int *lexicalOrder(int n, int *returnSize)
{
	int *s = malloc(n * sizeof(int));
	int size = 0;
	*returnSize = n;
	DFS(s, &size, n, 0);
	return s;
}

#else

int *lexicalOrder(int n, int *returnSize)
{
	int i;
	int current = 1;
	int *result;

	result = (int *) malloc(n * sizeof(int));
	i = -1;
	while (++i < n)
	{
		result[i] = current;
		if (current * 10 <= n) current *= 10;
		else
		{
			if (current >= n) current /= 10;
			current += 1;
			while (current % 10 == 0)
				current /= 10;
		}
	}
	*returnSize = n;
	return result;
}

#endif
