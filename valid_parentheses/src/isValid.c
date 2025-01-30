/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 17:29:40 by abelov            #+#    #+#             */
/*   Updated: 2025/01/28 17:29:45 by abelov           ###   ########.fr       */
/*                                                                            */
#include <string.h>
#include <alloca.h>
#include <limits.h>

/* ************************************************************************** */

typedef enum bool
{
	false = 0,
	true = 1
} bool;

/**
 * 20. Valid Parentheses
 *
 * Given a string s containing just the characters '(', ')', '{', '}',
 * 	'[' and ']', determine if the input string is valid.
 * @param s
 * @return
 */
bool isValid(const char *s);

bool isValid(const char *s)
{
	char *stack = (char *) alloca(strlen(s) * sizeof(char) + 1);
	int top = -1;

	while (*s)
	{
		char c = *s;
		if (c == '(' || c == '{' || c == '[')
			stack[++top] = c;
		else if (c == ')' || c == '}' || c == ']')
		{
			char opening = (char) (c - (c == ')' ? 1 : 2));
			if (top >= 0 && opening == stack[top]) top--;
			else
				return (false);
		}
		s++;
	}
	if (top >= 0)
		return (false);
	return (true);
}