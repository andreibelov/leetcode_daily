/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 23:12:27 by abelov            #+#    #+#             */
/*   Updated: 2025/02/23 23:12:27 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 119. Pascal's Triangle II
 *
 * Given an integer rowIndex, return the rowIndexth (0-indexed) row
 * of the Pascal's triangle.
 *
 * Constraints:
 * 	0 <= rowIndex <= 33
 */
int* getRow(int rowIndex, int* returnSize);

struct s_test_case
{
	int		rowIndex;
	Array	expected;
};

int ft_do_test(struct s_test_case *input)
{
	int *result;
	int resultSize;
	int check_val;

	result = getRow(input->rowIndex,  &resultSize);
	check_val = (input->expected.size == resultSize);
	if (!check_val)
	{
		printf("resultSize doesn't match expected value\n");
		printf("got \"%d\" whilst \"%d\" was to be expected\n",
			   resultSize, input->expected.size);
		check(false);
	}
	else
	{
		check_val = !memcmp(input->expected.arr, result, input->expected.size);

		if (!check_val)
		{
			printf("got ");
			ft_print_int_tab(result, resultSize, " whilst ");
			ft_print_int_tab(input->expected.arr, input->expected.size, "");
			printf(" was to be expected \n");
		}
	}
	check(check_val);
	free(result);
	return (0);
}

int main(void)
{
	int i;
	struct s_test_case cases[] = {
		{
			.rowIndex = 3,
			.expected = {(int[4]){1,3,3,1}, .size = 4}
		},
		{
			.rowIndex = 0,
			.expected = {(int[1]){1}, .size = 1}
		},
		{
			.rowIndex = 1,
			.expected = {(int[2]){1, 1}, .size = 2}
		},
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
