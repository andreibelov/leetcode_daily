/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   longestCommonPrefix.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:17:57 by abelov            #+#    #+#             */
/*   Updated: 2024/09/24 19:17:58 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * 3043. Find the Length of the Longest Common Prefix
 *
 * You are given two arrays with positive integers arr1 and arr2.
 *
 * You need to find the length of the longest common prefix between all pairs
 * of integers (x, y) such that x belongs to arr1 and y belongs to arr2.
 *
 * Return the length of the longest common prefix among all pairs.
 * If no common prefix exists among them, return 0.
 */
int longestCommonPrefix(const int *arr1, int arr1Size, const int *arr2, int arr2Size);

#include <math.h>

#define HASHMAP_SIZE 1000033 // Size of the hash map
#define HASH_MOD 100000007   // Large prime modulus for hashing

typedef struct
{
	int value; // Original value
	int exists; // Existence flag
} HashEntry;

unsigned int hash(int key)
{
	unsigned int result = (key % HASH_MOD + HASH_MOD) % HASH_MOD;
	return result % HASHMAP_SIZE;
}

int longestCommonPrefix(const int *arr1, int arr1Size, const int *arr2, int arr2Size)
{
	int ans = 0;
	int currentPrefix;
	int commonPrefixLength;
	unsigned int index;
	HashEntry prefixSet[HASHMAP_SIZE] = {0}; // Statically allocated

	// Store all prefixes from arr1
	int i = 0;
	while (i < arr1Size)
	{
		currentPrefix = arr1[i];
		while (currentPrefix > 0)
		{
			index = hash(currentPrefix);
			if (prefixSet[index].exists == 0)
			{
				prefixSet[index].value = currentPrefix;
				prefixSet[index].exists = 1;
			}
			currentPrefix /= 10; // Reduce the prefix
		}
		i++; // Increment index for arr1
	}

	// Check common prefixes in arr2
	i = -1;
	while (++i < arr2Size)
	{
		currentPrefix = arr2[i];
		while (currentPrefix > 0)
		{
			index = hash(currentPrefix);
			if (prefixSet[index].exists && prefixSet[index].value == currentPrefix)
			{
				commonPrefixLength = (int) (log10(currentPrefix) + 1);
				if (commonPrefixLength > ans)
					ans = commonPrefixLength;
				break; // Stop at the first match
			}
			currentPrefix /= 10; // Reduce the prefix
		}
	}

	return ans;
}
