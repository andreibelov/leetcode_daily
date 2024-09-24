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
int longestCommonPrefix(const int *arr1, int arr1Size,
						const int *arr2, int arr2Size);

#if defined USE_LINEAR_PROBING_HASH

#include <stdlib.h>
#include <math.h>

#define HASHMAP_SIZE 1000033 // Size of the hash map
#define HASH_MOD 100000007   // Large prime modulus for hashing

typedef struct HashEntry
{
	int value; // Original value
	int isOccupied; // Flag to indicate if the slot is occupied
} HashEntry;


unsigned int hash(int key)
{
	return (key % HASH_MOD + HASH_MOD) % HASH_MOD % HASHMAP_SIZE;
}

int longestCommonPrefix(const int *arr1, int arr1Size,
						const int *arr2, int arr2Size)
{
	int ans = 0;
	int currentPrefix;
	int commonPrefixLength = 0;
	HashEntry prefixSet[HASHMAP_SIZE] = {0};
	unsigned int index;

	// Store all prefixes from arr1
	int i = -1;
	while (++i < arr1Size)
	{
		currentPrefix = arr1[i];
		while (currentPrefix > 0)
		{
			index = hash(currentPrefix);
			// Linear probing for insertion
			while (prefixSet[index].isOccupied)
				index = (index + 1) % HASHMAP_SIZE; // Find next available slot
			prefixSet[index].value = currentPrefix;
			prefixSet[index].isOccupied = 1; // Mark the slot as occupied
			currentPrefix /= 10; // Reduce the prefix
		}
	}

	// Check common prefixes in arr2
	i = -1;
	while (++i < arr2Size)
	{
		currentPrefix = arr2[i];
		while (currentPrefix > 0)
		{
			index = hash(currentPrefix);
			// Linear probing for searching
			while (prefixSet[index].isOccupied)
			{
				if (prefixSet[index].value == currentPrefix)
				{
					commonPrefixLength = (int) (log10(currentPrefix) + 1);
					if (commonPrefixLength > ans)
						ans = commonPrefixLength;
					break; // Stop at the first match
				}
				index = (index + 1) % HASHMAP_SIZE; // Continue probing
			}
			currentPrefix /= 10; // Reduce the prefix
		}
	}
	return ans;
}

#elif defined USE_CHAINING

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HASHMAP_SIZE 1000033 // Size of the hash map
#define HASH_MOD 100000007   // Large prime modulus for hashing

typedef struct HashEntry {
	int value; // Original value
	struct HashEntry *next; // Pointer to the next entry in case of collision
} HashEntry;

unsigned int hash(int key) {
	unsigned int result = (key % HASH_MOD + HASH_MOD) % HASH_MOD;
	return result % HASHMAP_SIZE;
}

int longestCommonPrefix(const int *arr1, int arr1Size, const int *arr2, int arr2Size) {
	int ans = 0;
	HashEntry *prefixSet[HASHMAP_SIZE] = {NULL}; // Statically allocated array of pointers

	// Store all prefixes from arr1
	int i = 0;
	while (i < arr1Size) {
		int currentPrefix = arr1[i];
		while (currentPrefix > 0) {
			unsigned int index = hash(currentPrefix);
			HashEntry *entry = prefixSet[index];
			int exists = 0;

			// Check for existing prefix in the linked list
			while (entry != NULL) {
				if (entry->value == currentPrefix) {
					exists = 1; // Prefix already exists
					break;
				}
				entry = entry->next;
			}

			// Insert new prefix if it doesn't exist
			if (!exists) {
				HashEntry *newEntry = (HashEntry *)malloc(sizeof(HashEntry));
				newEntry->value = currentPrefix;
				newEntry->next = prefixSet[index]; // Insert at the head
				prefixSet[index] = newEntry;
			}
			currentPrefix /= 10; // Reduce the prefix
		}
		i++; // Increment index for arr1
	}

	// Check common prefixes in arr2
	i = 0;
	while (i < arr2Size) {
		int currentPrefix = arr2[i];
		while (currentPrefix > 0) {
			unsigned int index = hash(currentPrefix);
			HashEntry *entry = prefixSet[index];

			// Check for the prefix in the linked list
			while (entry != NULL) {
				if (entry->value == currentPrefix) {
					int commonPrefixLength = (int)(log10(currentPrefix) + 1);
					if (commonPrefixLength > ans) {
						ans = commonPrefixLength;
					}
					break; // Stop at the first match
				}
				entry = entry->next;
			}
			currentPrefix /= 10; // Reduce the prefix
		}
		i++; // Increment index for arr2
	}

	// Free allocated memory for hash entries
	int j = 0;
	while (j < HASHMAP_SIZE) {
		HashEntry *entry = prefixSet[j];
		while (entry != NULL) {
			HashEntry *temp = entry;
			entry = entry->next;
			free(temp);
		}
		j++; // Increment index for freeing
	}

	return ans;
}

#else

#include <math.h>

#define HASHMAP_SIZE 1000033 // Size of the hash map

typedef struct
{
	int value; // Original value
	int exists; // Existence flag
} HashEntry;

unsigned int hash(int key);

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

#define HASH_MOD 100000007   // Large prime modulus for hashing

#if defined USE_COMBINED_HASH
/**
 * combination of bit-shifting and multiplication
 */
unsigned int hash(int key)
{
	unsigned int hash = key;
	hash = ((hash >> 16) ^ hash) * 0x45d9f3b; // A large prime
	hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
	hash = (hash >> 16) ^ hash;
	return hash % HASHMAP_SIZE;
}
#elif defined USE_MID_SQUARE_HASH
#define HASH_BITS 10         // Number of middle bits to extract (adjust as needed)

/**
 * mid-square method
 */
unsigned int hash(int key)
{
	// Step 1: Square the key
	long long square = (long long)key * key;

	// Step 2: Extract middle bits from the squared result
	// Shift right to remove the lower bits and isolate the middle portion
	unsigned int middleBits = (square >> (HASH_BITS / 2)) % HASHMAP_SIZE;

	return middleBits;
}

#elif defined USE_TRIPLE_PRIME_HASH

unsigned int hash(int key)
{
	// Use a larger prime for the modulus
	const unsigned int prime1 = 73856093;
	const unsigned int prime2 = 19349663;
	const unsigned int prime3 = 83492791;

	// Generate the hash value
	unsigned int result = (key * prime1) ^ (key * prime2) ^ (key * prime3);
	return result % HASHMAP_SIZE;
}
#else

unsigned int hash(int key)
{
	/* Ensure non-negative result */
	unsigned int result = (key % HASH_MOD + HASH_MOD) % HASH_MOD;
	return (result % HASHMAP_SIZE);
}
#endif

#endif
