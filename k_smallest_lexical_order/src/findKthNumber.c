/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findKthNumber.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:50:52 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 13:50:53 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/**
 * 440. K-th Smallest in Lexicographical Order
 *
 * Given two integers n and k,
 * return the kth lexicographically smallest integer in the range [1, n].
 */
int findKthNumber(int n, int k);

#if defined USE_EDITORIAL
/**
 * This approach uses a more mathematical and efficient calculation of how
 * many numbers exist in a given range in lexicographical order.
 * It uses a structured "step-based" traversal of the lexicographical tree.
 *
 * Iterative and Simple:
 * It is more concise, focusing on iterating through potential prefixes and
 * efficiently counting the possible numbers that start with a given prefix.
 */

#define min(a, b) (((a) < (b)) ? (a) : (b))

/**
 * function count_steps() calculates the total count of numbers between
 * result and result + 1 that could be considered candidates.
 */
int count_steps(long long n, long long lower, long long upper)
{
	long long steps = 0;
	while (lower <= n)
	{
		steps += min(n + 1, upper) - lower;
		lower *= 10;
		upper *= 10;
	}
	return ((int)steps);
}

int findKthNumber(int n, int k)
{
	int steps;
	long long result = 1;
	k -= 1;

	while (k > 0)
	{
		steps = count_steps(n, result, result + 1);
		if (steps <= k)
		{
			result++;
			k -= steps;
		}
		else
		{
			result *= 10;
			k--;
		}
	}
	return ((int)result);
}

#else

/**
 * This approach solution explicitly compares each potential candidate with
 * the target and adjusts the number of valid candidates accordingly. It has
 * more explicit control over each level of the digits and keeps track of
 * detailed information (such as the remaining valid numbers).
 */

#define MAX_STACK_SIZE 10000

static inline int *fillCounts(int *counts)
{
	int i = 0;

	// Fill counts array to count possible numbers for each prefix level
	while (++i <= 9)
		counts[i] = counts[i - 1] * 10 + 1;
	return (counts);
}

static inline  int *splitNum(int i, int *const nums, int *numsSize)
{
	int size = *numsSize;

	while (i > 0)
	{
		nums[size++] = i % 10;  // Store the last digit of 'n'
		i /= 10;                    // Remove the last digit of 'n'
	}
	*numsSize = size;
	return nums;
}

struct Triplet
{
	int total;
	int k;
	int result;
} tryAllDigits(int target, int i, int start, int *const counts, struct Triplet t)
{
	int candidate;                  // Current candidate number being evaluated
	int num;                        // Number of possible candidates for the current prefix
	int j = start - 1;

	while (++j <= 9)
	{
		candidate = t.result * 10 + j;  // Build the candidate number by appending 'j' to 'result'

		// Determine the number of valid numbers starting with this candidate
		if (candidate < target)
			num = counts[i + 1];      // If candidate is less than the target, all numbers are valid
		else if (candidate > target)
			num = counts[i];          // If candidate is greater than the target, no numbers are valid
		else
			num = t.total - counts[i + 1] * (j - start) - counts[i] * (9 - j); // Partial valid numbers

		// Check if 'k' is greater than the number of valid candidates for this prefix
		if (t.k > num)
			t.k -= num;                 // If yes, skip those candidates and adjust 'k'
		else
		{
			t.result = candidate;       // If no, we found the valid candidate for this level
			--t.k;                      // Decrease 'k' as we have narrowed down the search
			t.total = num - 1;          // Adjust the total remaining numbers
			break;                    // Exit the loop and move to the next digit
		}
	}
	return (t);
}

int findKthNumber(int n, int k)
{
	struct Triplet t = {.total = n, .k = k, .result = 0};
	int target = 0;                 // Current target number we are building
	int i;
	int start;                      // Start digit for each level (usually 0 or 1)

	int numsSize = 0;
	// Convert 'n' into an array of digits stored in reverse order in 'nums'
	int *const nums = splitNum(n, (int[MAX_STACK_SIZE]){0}, &numsSize);

	// Precomputed counts for numbers at each digit level starting with 1 to 9
	int *const counts = fillCounts((int[10]){0});

	// Process each digit from the most significant down to the least significant
	i = numsSize;
	while (--i >= 0 && t.k)
	{
		target = target * 10 + nums[i]; // Build the target number from the most significant digit down

		// Determine the starting digit for the current level (1 for most significant, otherwise 0)
		start = (i == numsSize - 1) ? 1 : 0;

		// Try all digits from 'start' to 9 to find the valid k-th number
		t = tryAllDigits(target, i, start, counts, t);
	}
	return (t.result);  // Return the final result, which is the k-th smallest number
}

#endif
