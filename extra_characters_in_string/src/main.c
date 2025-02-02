/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:58:31 by abelov            #+#    #+#             */
/*   Updated: 2024/09/24 00:58:31 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 2707. Extra Characters in a String
 *
 * You are given a 0-indexed string s and a dictionary of words dictionary.
 * You have to break s into one or more non-overlapping substrings such that
 * each substring is present in dictionary. There may be some extra characters
 * in s which are not present in any of the substrings.
 *
 * Return the minimum number of extra characters left over
 * if you break up s optimally.
 *
 * Constraints:
 * 	1 <= s.length <= 50
 * 	1 <= dictionary.length <= 50
 * 	1 <= dictionary[i].length <= 50
 * 	dictionary[i] and s consists of only lowercase English letters
 * 	dictionary contains distinct words
 */
int minExtraChar(char *s, char **dictionary, int dictionarySize);

typedef struct s_test_case
{
	const char	*s;
	StringArray	dictionary;
	int			expected;
}	Input;

int ft_do_test(Input *input)
{
	int result;
	int check_val;

	result = minExtraChar((char *)input->s, input->dictionary.arr, input->dictionary.size);
	check_val = (input->expected == result);
	if (!check_val)
		printf("got \"%d\" whilst \"%d\" was to be expected\n",
			   result, input->expected);
	check(check_val);
	return (0);
}

int main(void)
{
	int				i;
	Input	inputs[] = {
		{
			.s = "leetscode",
			.dictionary = {.arr = (char **)(const char *[3]){"leet", "code", "leetcode"}, .size = 3},
			.expected = 1
		},
		{
			.s = "sayhelloworld",
			.dictionary = {.arr = (char **)(const char *[2]){"hello","world"}, .size = 2},
			.expected = 3
		},
	};

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) (sizeof(inputs) / sizeof(inputs[0])))
		ft_do_test(&inputs[i]);
	return (EXIT_SUCCESS);
}

