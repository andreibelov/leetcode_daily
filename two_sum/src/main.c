/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 01:10:56 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 01:10:57 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 1. Two Sum
 *
 * Constraints:
 * 	2 <= nums.length <= 10^4
 * 	-10^9 <= nums[i] <= 10^9
 * 	-10^9 <= target <= 10^9
 * 	Only one valid answer exists.
 */
int *twoSum(int *nums, int numsSize, int target, int *returnSize);

struct s_test_case
{
	int target;
	Array	nums;
	Array expected;
};

int ft_do_test(struct s_test_case *input)
{
	Array result;
	int check_val;
	ft_print_int_tab(input->nums.arr, input->nums.size, NULL);

	result.arr = twoSum(input->nums.arr, input->nums.size,
					input->target, &result.size);
	if (input->expected.size != result.size)
	{
		printf("resultSize doesn't match expected value\n");
		printf("got \"%d\" whilst \"%d\" was to be expected\n",
			   result.size, input->expected.size);
		check(false);
	}
	else
	{
		check_val = !memcmp(input->expected.arr, result.arr, input->expected.size * sizeof(int));
		if (!check_val)
		{
			printf("got ");
			ft_print_int_tab(result.arr, result.size, " whilst ");
			ft_print_int_tab(input->expected.arr, input->expected.size, "");
			printf(" was to be expected \n");
		}
		check(check_val);
	}
	free(result.arr);
	return (0);
}

int main(void)
{
	int i;
	struct s_test_case cases[] = {
		{
			.target = 9,
			.nums = {(int[4]) {2,7,11,15}, .size = 4},
			.expected = {(int[2]) {0,1}, .size = 2}
		},
		{
			.target = 6,
			.nums = {(int[3]) {3,2,4}, .size = 3},
			.expected = {(int[2]) {1,2}, .size = 2}
		},
		{
			.target = 6,
			.nums = {(int[2]) {3,3}, .size = 2},
			.expected = {(int[2]) {0,1}, .size = 2}
		}
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
