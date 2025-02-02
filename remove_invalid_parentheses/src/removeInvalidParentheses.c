/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removeInvalidParentheses.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 00:19:14 by abelov            #+#    #+#             */
/*   Updated: 2025/01/29 00:19:14 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <alloca.h>
#include <string.h>
#include <uthash.h>

/**
 * 301. Remove Invalid Parentheses
 *
 * Given a string s that contains parentheses and letters, remove the minimum
 * number of invalid parentheses to make the input string valid.
 *
 * Return a list of unique strings that are valid with the minimum number
 * of removals. You may return the answer in any order.
 *
 * Note: The returned array must be malloced, assume caller calls free().
 */
char **removeInvalidParentheses(char *s, int *returnSize);

typedef struct HashSetEntry
{
	char *key;
	UT_hash_handle hh;
} hash_t;

typedef struct StackFrame
{
	int index;
	int leftCount;
	int rightCount;
	int leftAccumulated;
	int rightAccumulated;
	int pos;
}	StackFrame;

hash_t *hash = NULL;


/*
 * Could've used HASH_ADD_STR(*t, key, cur);
 */
void hash_insert(hash_t **t, const char *str)
{
	hash_t *cur;

	HASH_FIND_STR(*t, str, cur);
	if (!cur)
	{
		cur = (hash_t *) malloc(sizeof(hash_t)), cur->key = strdup(str);
		HASH_ADD_KEYPTR(hh, *t, cur->key, strlen(cur->key), cur);
	}
}

void searchValidExpressions(char *s, int len, int leftCount, int rightCount)
{
	char *currentExpression = (char *) alloca((len + 1) * sizeof(char));

	// Stack for iterative DFS
	StackFrame *sf, *stack = (StackFrame *) malloc(len * 10 * sizeof(StackFrame));
	int ss = 0;

	// Initial state
	stack[ss++] = (StackFrame){.leftCount = leftCount, .rightCount = rightCount};
	while (ss > 0)
	{
		StackFrame state = stack[--ss];

		if (state.index == len)
		{
			if (state.leftCount == 0 && state.rightCount == 0)
			{
				currentExpression[state.pos] = '\0';
				hash_insert(&hash, currentExpression);
			}
			continue;
		}

		// Early termination if not enough characters left
		if (len - state.index < state.leftCount + state.rightCount
			|| state.leftAccumulated < state.rightAccumulated)
			continue;

		char ch = s[state.index++];

		// Skip '(' if removable
		if (ch == '(' && state.leftCount > 0)
			sf = &stack[ss++], *sf = state, sf->leftCount--;

		// Skip ')' if removable
		if (ch == ')' && state.rightCount > 0)
			sf = &stack[ss++], *sf = state, sf->rightCount--;

		// Include current character
		currentExpression[state.pos] = ch;
		sf = &stack[ss++], *sf = state, sf->pos++;
		sf->leftAccumulated += (ch == '(');
		sf->rightAccumulated += (ch == ')');
	}
	free(stack);
}

void searchValidExpressionsRec(char *s, int len, int index, int leftCount,
							   int rightCount, int leftAccumulated,
							   int rightAccumulated, char *currentExpression, int pos)
{
	if (index == len)
	{
		if (leftCount == 0 && rightCount == 0)
		{
			currentExpression[pos] = '\0';
			hash_insert(&hash, currentExpression);
		}
		return;
	}

	// Early termination if we don't have enough parentheses left to remove
	if (len - index < leftCount + rightCount || leftAccumulated < rightAccumulated)
		return;

	// If it's a left parenthesis, and we can remove it, recurse without including it
	if (s[index] == '(' && leftCount)
		searchValidExpressionsRec(s, len, index + 1, leftCount - 1, rightCount, leftAccumulated, rightAccumulated,
							   currentExpression, pos);

	// If it's a right parenthesis, and we can remove it, recurse without including it
	if (s[index] == ')' && rightCount)
		searchValidExpressionsRec(s, len, index + 1, leftCount, rightCount - 1, leftAccumulated, rightAccumulated,
							   currentExpression, pos);

	// Determine whether the current character is a left or right parenthesis
	int increaseLeft = s[index] == '(' ? 1 : 0;
	int increaseRight = s[index] == ')' ? 1 : 0;

	currentExpression[pos] = s[index];
	// Recurse to the next character in the string, including the current character in the expression
	searchValidExpressionsRec(s, len, index + 1, leftCount, rightCount, leftAccumulated + increaseLeft,
						   rightAccumulated + increaseRight, currentExpression, pos + 1);
}

void dfs(hash_t **t, char *s, int leftCount, int rightCount, char res[], int pos, int weight)
{
	if (*s)
	{
		if (*s == ')')
		{
			if (weight)
			{
				res[pos] = *s;
				dfs(t, s + 1, leftCount, rightCount, res, pos + 1, weight - 1);
			}
			if (rightCount)
				dfs(t, s + 1, leftCount, rightCount - 1, res, pos, weight);
		}
		else
		{
			res[pos] = *s;
			dfs(t, s + 1, leftCount, rightCount, res, pos + 1, weight + (*s == '('));

			if (*s == '(' && leftCount)
				dfs(t, s + 1, leftCount - 1, rightCount, res, pos, weight);
		}
	}
	else if (leftCount == 0 && rightCount == 0) res[pos] = '\0', hash_insert(t, res);
}

char **removeInvalidParentheses(char *s, int *returnSize)
{
	int leftCount = 0, rightCount = 0;
	int i;

	i = -1;
	while (++i > -1 && s[i])
	{
		if (s[i] == '(') leftCount++;
		else if (s[i] == ')')
		{
			if (leftCount > 0) leftCount--;
			else rightCount++;
		}
	}

	searchValidExpressions(s, i, leftCount, rightCount);

	*returnSize = HASH_COUNT(hash);
	char **ans = (char **) malloc((*returnSize) * sizeof(char *));

	i = 0;
	hash_t *cur = NULL, *next = NULL;
	HASH_ITER(hh, hash, cur, next)
	{
		HASH_DEL(hash, cur);
		ans[i++] = cur->key, free(cur);
	}
	hash = NULL;
	return (ans);
}
