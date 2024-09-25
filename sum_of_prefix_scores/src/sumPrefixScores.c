/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sumPrefixScores.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 21:40:37 by abelov            #+#    #+#             */
/*   Updated: 2024/09/25 21:40:38 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

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

/* since we are using a standard lowercase alphabet ('a' to 'z'),
 * the size of the next array should be 26. */
#define MAX_TRIE_NODES 26
#define STACK_SIZE 10000  // Adjust if needed for larger trie sizes

typedef struct TrieNode
{
	int cnt;
	struct TrieNode *next[MAX_TRIE_NODES];
} TrieNode;

void freeTrie(TrieNode *root)
{
	int i;
	int sp = 0;
	TrieNode *stack[STACK_SIZE];
	TrieNode *current;

	if (root)
		stack[sp++] = root;

	while (sp)
	{
		current = stack[--sp];

		i = -1;
		while (++i < MAX_TRIE_NODES)
			if (current->next[i])
				stack[sp++] = current->next[i];
		free(current);
	}
}

void insert(char *word, TrieNode *node)
{
	if (!word || !node)
		return;
	while (*word)
	{
		if (!node->next[*word - 'a'])
			node->next[(*word - 'a')] = (TrieNode *) calloc(1, sizeof(TrieNode));
		node->next[*word - 'a']->cnt++;
		node = node->next[*word++ - 'a'];
	}
}

/**
 * Calculate the prefix count using this function.
 */
int count(char *s, TrieNode *node)
{
	int ans = 0;
	// The ans would store the total sum of counts.
	while (*s && node->next[*s - 'a'])
	{
		ans += node->next[*s - 'a']->cnt;
		node = node->next[*s++ - 'a'];
	}
	return ans;
}

int *sumPrefixScores(char **words, int wordsSize, int *returnSize)
{
	int i;
	int n = wordsSize;
	int *scores = (int *)malloc(n * sizeof(int));
	TrieNode *root = (TrieNode *) calloc(1, sizeof(TrieNode));

	i = -1;
	while (++i < n)
		insert(words[i], root);

	i = -1;
	while (++i < n)
		scores[i] = count(words[i], root);
	*returnSize = n;
	freeTrie(root);
	return (scores);
}
