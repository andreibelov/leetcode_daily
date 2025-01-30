/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:16:47 by abelov            #+#    #+#             */
/*   Updated: 2025/01/29 00:16:48 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 301. Remove Invalid Parentheses
 *
 * Given a string s that contains parentheses and letters, remove the minimum
 * number of invalid parentheses to make the input string valid.
 *
 * Constraints:
 * 	1 <= s.length <= 25
 * 	s consists of lowercase English letters and parentheses '(' and ')'.
 * 	There will be at most 20 parentheses in s.
 */
char **removeInvalidParentheses(const char *s, int *returnSize);

typedef struct s_test_case
{
	const char *string;
	StringArray	expected;
}	Input;

int ft_do_test(Input *input)
{
	int i;
	char **result;
	int resultSize;
	int check_val;

	printf("\"%s\"\n", input->string);

	result = removeInvalidParentheses(input->string, &resultSize);
	check_val = (input->expected.size == resultSize);
	if (!check_val)
		printf("got array of size \"%d\" whilst \"%d\" was to be expected\n",
			   resultSize, input->expected.size);
	else
	{
		char *(*expected)[input->expected.size] = (void *)input->expected.arr;
		char *(*ans)[resultSize] = (void *)result;

		qsort(input->expected.arr, input->expected.size, sizeof(char *), compare_strings);
		qsort(result, resultSize, sizeof(char *), compare_strings);

		i = -1;
		while(++i < resultSize)
		{
			check_val = !strcmp(input->expected.arr[i], result[i]);
			if (!check_val) break;
		}

		if (!check_val)
		{
			printf("got ");
			ft_print_str_tab(result, resultSize, " whilst ");
			ft_print_str_tab(input->expected.arr, input->expected.size, "");
			printf(" was to be expected \n");
		}
	}
	check(check_val);
	i = -1;
	while (++i < resultSize)
		free(result[i]);
	free(result);
	return (0);
}

int main(void)
{
	int i;
	Input cases[] = {
		{
			.string = "()())()",
			.expected = {(char **) (const char *[2]) {"(())()", "()()()" }, .size = 2}
		},
		{
			.string = "(a)())()",
			.expected = {(char **) (const char *[2]) {"(a())()", "(a)()()" }, .size = 2}
		},
		{
			.string = ")(",
			.expected = {(char **) (const char *[1]) { "" }, .size = 1}
		},
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
