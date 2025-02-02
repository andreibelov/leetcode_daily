/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:50:27 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 13:50:28 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 440. K-th Smallest in Lexicographical Order
 *
 * Given two integers n and k,
 * return the kth lexicographically smallest integer in the range [1, n].
 *
 * Constraints:
 * 	1 <= k <= n <= 10^9
 */
int findKthNumber(int n, int k);

typedef struct s_test_case
{
	int n;
	int k;
	int expected;
}	Input;

int ft_do_test(Input *input)
{
	int result;
	int check_val;

	result = findKthNumber(input->n, input->k);
	check_val = (input->expected == result);
	if (!check_val)
		printf("got \"%d\" whilst \"%d\" was to be expected\n",
			   result, input->expected);
	check(check_val);
	return (0);
}

int main(void)
{
	int i;
	Input cases[] = {
		{
			.n = 13,
			.k = 2,
			.expected = 10
		},
		{
			.n = 1,
			.k = 1,
			.expected = 1
		},
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
