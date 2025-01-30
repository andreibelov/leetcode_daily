/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:29:17 by abelov            #+#    #+#             */
/*   Updated: 2025/01/28 17:29:18 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "leetcode_daily.h"

/**
 * 20. Valid Parentheses
 *
 * Given a string s containing just the characters '(', ')', '{', '}',
 * 	'[' and ']', determine if the input string is valid.
 * @param s
 * @return
 */
bool isValid(const char* s);

typedef struct s_input
{
	const char	*str;
	int 		expected;
}	Input;

int ft_do_test(Input *input)
{
	int	result;
	int	check_val;

	result = isValid(input->str);
	check_val = input->expected == result;
	if (!check_val)
	{
		printf("for string \"%s\" ", input->str);
		printf("got \"%s\" whilst \"%s\" was to be expected\n",
			   result ? "valid" : "invalid", input->expected ? "valid" : "invalid");
	}
	check(check_val);
	return (0);
}

int main(void)
{
	int		i;
	Input	inputs[] = {
		{ .str = "()", .expected = true },
		{ .str = "()[]{}", .expected = true },
		{ .str = "(]", .expected = false },
		{ .str = "([])", .expected = true },
		{ .str = "[", .expected = false }
	};

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) (sizeof(inputs) / sizeof(inputs[0])))
		ft_do_test(&inputs[i]);
	return (EXIT_SUCCESS);
}

