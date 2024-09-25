/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 21:43:45 by abelov            #+#    #+#             */
/*   Updated: 2024/09/25 21:43:46 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 2416. Sum of Prefix Scores of Strings
 *
 * You are given an array words of size n consisting of non-empty strings.
 *
 * Return an array answer of size n where answer[i] is the sum of scores
 * of every non-empty prefix of words[i].
 * Note that a string is considered as a prefix of itself.
 *
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *sumPrefixScores(char **words, int wordsSize, int *returnSize);

struct s_test_case
{
	StringArray	words;
	Array expected;
};

int ft_do_test(struct s_test_case *input)
{
	int *result;
	int resultSize;
	int check_val;

	ft_print_str_tab(input->words.arr, input->words.size, "\n");

	result = sumPrefixScores(input->words.arr,  input->words.size, &resultSize);
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
			.words = {(char **) (const char *[4]) {"abc","ab","bc","b"}, .size = 4},
			.expected = {(int[4]){5,4,3,2}, .size = 4}
		},
		{
			.words = {(char **) (const char *[1]) {"abcd"}, .size = 1},
			.expected = {(int[1]){4}, .size = 1}
		}
	};
	size_t inputs_size = (sizeof(cases) / sizeof(cases[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&cases[i]);
	return (EXIT_SUCCESS);
}
