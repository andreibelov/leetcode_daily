/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minExtraChar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 00:58:45 by abelov            #+#    #+#             */
/*   Updated: 2024/09/24 00:58:45 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

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
 */
int minExtraChar(char *s, char **dictionary, int dictionarySize);

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
int minExtraChar(char *s, char **dictionary, int dictionarySize)
{
	int i,j,k,h;
	int length = (int)strlen(s) + 1;
	int* dp = (int *)malloc(length * sizeof(int));

	i = -1;
	while (++i < length)
		dp[i] = i;

	i = 0;
	while (++i < length)
	{
		dp[i] = min(dp[i], dp[i - 1] + 1);

		j = -1;
		while (++j < dictionarySize)
		{
			if (i >= (int)strlen(dictionary[j]))
			{
				h = i - 1;
				k = (int)(strlen(dictionary[j]) - 1);
				while (dictionary[j][k--] == s[h--])
				{
					if (k == -1)
					{
						dp[i] = min(dp[i], dp[h + 1]);
						break;
					}
				}
			}
		}
	}
	i = dp[length - 1];
	free(dp);
	return i;
}
