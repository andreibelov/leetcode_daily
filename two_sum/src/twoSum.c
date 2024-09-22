/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twoSum.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 01:11:59 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 01:12:00 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <uthash.h>

typedef struct
{
	long key;
	int value;
	UT_hash_handle hh;
}	hash_entry;

/**
 * 1. Two Sum
 *
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
	int i;
	int *result;
	int complement;
	hash_entry *item;
	hash_entry *tmp;
	hash_entry *table = NULL;

	i = -1;
	while (++i < numsSize)
	{
		complement = target - nums[i];
		HASH_FIND_INT(table, &complement, item);
		if (item)
		{
			result = malloc(sizeof(int) * 2);
			result[0] = item->value;
			result[1] = i;
			*returnSize = 2;
			/* Free the hash table */
			HASH_ITER(hh, table, item, tmp) {
				HASH_DEL(table, item);
				free(item);
			}
			HASH_CLEAR(hh, table);
			return (result);
		}
		item = malloc(sizeof(hash_entry));
		item->key = nums[i];
		item->value = i;
		HASH_ADD_INT(table, key, item);
	}
	/* Free the hash table */
	HASH_ITER(hh, table, item, tmp) {
		HASH_DEL(table, item);
		free(item);
	}
	HASH_CLEAR(hh, table);
	/* Return an empty array if no solution is found */
	return (*returnSize = 0, malloc(0));  // Allocate 0 bytes
}
