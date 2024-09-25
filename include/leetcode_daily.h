/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leetcode_daily.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 00:53:55 by abelov            #+#    #+#             */
/*   Updated: 2024/09/22 00:53:56 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEETCODE_DAILY_H
#define LEETCODE_DAILY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <limits.h>

typedef struct Array
{
	int *arr;
	int size;
}	Array;

typedef struct TwoDArray
{
	Array *arrays;
	int size;
}	TwoDArray;

typedef struct StringArray
{
	char **arr;
	int size;
}	StringArray;


enum
{
	null = INT_MIN
};

# define FT_RED   "\033[0;31m"
# define FT_GREEN "\033[0;32m"
# define FT_CYAN  "\033[36m"
# define FT_RESET "\e[0m"

void sigsegv(int signal)
{
	(void) signal;
	printf("> "FT_CYAN".SIGSEGV"FT_RESET"\n");
	exit(EXIT_SUCCESS);
}

void check(bool succes)
{
	if (succes)
		printf("> "FT_GREEN".OK "FT_RESET"\n");
	else
		printf("> "FT_RED".KO "FT_RESET"\n");
}

void ft_print_str_tab(char **tab,  size_t size, const char *eol)
{
	size_t pos;

	pos = 0;
	printf("[");
	if (pos < size)
		printf("\"%s\"", tab[pos++]);
	while (pos < size)
		printf(", \"%s\"", tab[pos++]);
	printf("]%s", (!eol) ? "\n" : eol);
}


void ft_print_int_tab(int tab[], size_t size, const char *eol)
{
	size_t pos;

	pos = 0;
	printf("[");
	if (pos < size)
		printf("%d", tab[pos++]);
	while (pos < size)
		printf(", %d", tab[pos++]);
	printf("]%s", (!eol) ? "\n" : eol);
}

void ft_print_int_tab_null(int tab[], size_t size, int nil, const char *eol)
{
	size_t pos;
	int val;

	pos = 0;
	printf("[");
	if (pos < size)
	{
		val = tab[pos++];
		if (val == nil)
			printf("%s", "null");
		else
			printf("%d", val);
	}
	while (pos < size)
	{
		val = tab[pos++];
		if (val == nil)
			printf(", %s", "null");
		else
			printf(", %d", val);
	}
	printf("]");
	if (!eol)
		printf("\n");
	else
		printf("%s", eol);
}

#define MAX_STACK_SIZE 10000
typedef struct TreeNode TreeNode;

struct TreeNode
{
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

struct TreeNode *new_node(int val)
{
	struct TreeNode *node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
	node->val = val;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

struct TreeNode *deserialize_level_order(int *arr, int size)
{
	struct TreeNode	*root = NULL;
	struct TreeNode	*current;
	struct TreeNode	*queue[MAX_STACK_SIZE];
	int				front = 0, rear = 0;
	int 			i = 0;

	if (size == 0 || arr[0] == null)
		return NULL;

	root = new_node(arr[0]);
	queue[rear++] = root;

	while (i < size) // Start from the second element in the array
	{
		current = queue[front++]; // Dequeue the front node

		if (++i < size && arr[i] != null)
		{
			current->left = new_node(arr[i]);
			queue[rear++] = current->left; // Enqueue the left child
		}
		if (++i < size && arr[i] != null)
		{
			current->right = new_node(arr[i]);
			queue[rear++] = current->right; // Enqueue the right child
		}
	}
	return (root);
}

#define NULL_VALUE null
#define MAX_QUEUE_SIZE MAX_STACK_SIZE

int *serialize_level_order_leet(struct TreeNode *root, int *arraySize) {
	struct TreeNode *currentNode;
	struct TreeNode *queue[MAX_QUEUE_SIZE];
	int front = 0, rear = 0;
	int *serializedArray = NULL;
	int tempStack[MAX_QUEUE_SIZE * 2];
	int stackPointer = 0;

	if (arraySize == NULL)
		return NULL;

	// Initialize level-order queue with root node
	if (root)
		queue[rear++] = root;
	else {
		*arraySize = 0;
		return NULL; // Return NULL for an empty tree
	}

	// Process the queue in level-order
	while (rear > front) {
		currentNode = queue[front++];

		if (currentNode) {
			tempStack[stackPointer++] = currentNode->val;

			// Always add children to maintain structure, even if they are null
			queue[rear++] = currentNode->left;
			queue[rear++] = currentNode->right;
		} else {
			tempStack[stackPointer++] = NULL_VALUE; // Represent null child as NULL_VALUE
		}
	}

	// Set the size of the serialized array
	*arraySize = stackPointer;

	// Trim trailing nulls from the serialization (LeetCode style)
	while (stackPointer > 0 && tempStack[stackPointer - 1] == NULL_VALUE) {
		stackPointer--;
	}

	*arraySize = stackPointer;

	// Allocate memory for the result array
	serializedArray = (int *) malloc(stackPointer * sizeof(int));
	if (serializedArray == NULL) {
		// Handle memory allocation failure
		return NULL;
	}

	// Copy the temp stack into the result array
	memcpy(serializedArray, tempStack, stackPointer * sizeof(int));

	return serializedArray;
}

int *serialize_level_order(struct TreeNode *root, int *arraySize)
{
	struct TreeNode	*node;
	struct TreeNode	*queue[MAX_STACK_SIZE];
	int				front = 0, rear = 0;
	int				*result = NULL;
	int				stack[MAX_STACK_SIZE * 2];
	int 			sp = 0;

	if (arraySize == NULL)
		return NULL;
	if (root)
		queue[rear++] = root;
	while (rear > front) // Start from the second element in the array
	{
		node = queue[front++];

		if (node)
		{
			stack[sp++] = node->val;
			if (node->left || node->right)
			{
				queue[rear++] = node->left;
				queue[rear++] = node->right;
			}
		}
		else
			stack[sp++] = null;
	}
	*arraySize = sp;
	result = (int *) malloc(sp * sizeof(int));
	memcpy(result, stack, sp * sizeof(int));
	return (result);
}

#define MAX_HEIGHT 10000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20) // hack? this seems defined somewhere.. math.h?

typedef struct asciiTree ASCIITree;

//adjust gap between left and right nodes
static int gap = 3;

struct asciiTree
{
	ASCIITree *left, *right;

	// length of the edge from this node to its children
	int edge_length;

	// height of node
	int height;

	//-1=I am left, 0=I am root, 1=right
	int parent_dir;

	// max supported unit32 in dec, 10 digits max
	char label[11];

	// label of length to be printed
	int label_length;
};

int print_next;

int MIN(int X, int Y)
{ return ((X) < (Y)) ? (X) : (Y); }

int MAX(int X, int Y)
{ return ((X) > (Y)) ? (X) : (Y); }

ASCIITree *build_ascii_tree_recursive(TreeNode *t)
{
	ASCIITree *node;

	if (t == NULL)
		return NULL;

	node = malloc(sizeof(ASCIITree));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);

	if (node->left != NULL)
	{
		node->left->parent_dir = -1;
	}

	if (node->right != NULL)
	{
		node->right->parent_dir = 1;
	}

	if (t->val != INT_MIN)
		sprintf(node->label, "%d", t->val);
	else
		sprintf(node->label, "%s", "null");
	node->label_length = strlen(node->label);

	return node;
}

ASCIITree *build_ascii_tree(TreeNode *t)
{
	ASCIITree *node;
	if (t == NULL)
		return NULL;
	node = build_ascii_tree_recursive(t);
	node->parent_dir = 0;
	return node;
}

void free_tree(TreeNode *root)
{
	TreeNode *node;
	TreeNode *stack[MAX_STACK_SIZE];
	int stackSize = 0;

	if (root == NULL) return;

	stack[stackSize++] = root;

	while (stackSize != 0)
	{
		node = stack[--stackSize];
		if (node->right != NULL)
			stack[stackSize++] = node->right;
		if (node->left != NULL)
			stack[stackSize++] = node->left;
		free(node);
	}
}

// Free all the nodes of the given tree
void free_ascii_tree(ASCIITree *node)
{
	if (node == NULL)
		return;
	free_ascii_tree(node->left);
	free_ascii_tree(node->right);
	free(node);
}

// The following function fills in the lprofile array for the given tree.
// It assumes that the center of the label of the root of this tree
// is located at a position (x,y).  It assumes that the edge_length
// fields have been computed for this tree.
void compute_lprofile(ASCIITree *node, int x, int y)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x - ((node->label_length - isleft) / 2));
	if (node->left != NULL)
	{
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
		{
			lprofile[y + i] = MIN(lprofile[y + i], x - i);
		}
	}
	compute_lprofile(node->left, x - node->edge_length - 1,
					 y + node->edge_length + 1);
	compute_lprofile(node->right, x + node->edge_length + 1,
					 y + node->edge_length + 1);
}

void compute_rprofile(ASCIITree *node, int x, int y)
{
	int i, notleft;
	if (node == NULL)
		return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x + ((node->label_length - notleft) / 2));
	if (node->right != NULL)
	{
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
		{
			rprofile[y + i] = MAX(rprofile[y + i], x + i);
		}
	}
	compute_rprofile(node->left, x - node->edge_length - 1,
					 y + node->edge_length + 1);
	compute_rprofile(node->right, x + node->edge_length + 1,
					 y + node->edge_length + 1);
}

// This function fills in the edge_length and
// height fields of the specified tree
void compute_edge_lengths(ASCIITree *node)
{
	int h, hmin, i, delta;
	if (node == NULL)
		return;
	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
	{
		node->edge_length = 0;
	}
	else
	{
		if (node->left != NULL)
		{
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
			{
				rprofile[i] = -INFINITY;
			}
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		else
		{
			hmin = 0;
		}
		if (node->right != NULL)
		{
			for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
			{
				lprofile[i] = INFINITY;
			}
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		else
		{
			hmin = 0;
		}
		delta = 4;
		for (i = 0; i < hmin; i++)
		{
			delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
		}
		// If the node has two children of height 1, then we allow the
		// two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
			 (node->right != NULL && node->right->height == 1)) &&
			delta > 4)
		{
			delta--;
		}

		node->edge_length = ((delta + 1) / 2) - 1;
	}

	// now fill in the height of node
	h = 1;
	if (node->left != NULL)
	{
		h = MAX(node->left->height + node->edge_length + 1, h);
	}
	if (node->right != NULL)
	{
		h = MAX(node->right->height + node->edge_length + 1, h);
	}
	node->height = h;
}

// This function prints the given level of the given tree, assuming
// that the node has the given x cordinate.
void print_level(ASCIITree *node, int x, int level)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	if (level == 0)
	{
		for (i = 0; i < (x - print_next - ((node->label_length - isleft) / 2)); i++)
		{
			printf(" ");
		}
		print_next += i;
		printf("%s", node->label);
		print_next += node->label_length;
	}
	else if (node->edge_length >= level)
	{
		if (node->left != NULL)
		{
			for (i = 0; i < (x - print_next - (level)); i++)
			{
				printf(" ");
			}
			print_next += i;
			printf("/");
			print_next++;
		}
		if (node->right != NULL)
		{
			for (i = 0; i < (x - print_next + (level)); i++)
			{
				printf(" ");
			}
			print_next += i;
			printf("\\");
			print_next++;
		}
	}
	else
	{
		print_level(node->left, x - node->edge_length - 1,
					level - node->edge_length - 1);
		print_level(node->right, x + node->edge_length + 1,
					level - node->edge_length - 1);
	}
}

// prints ascii tree for given BinaryTree structure
void print_tree(struct TreeNode *root)
{
	ASCIITree *proot;
	int xmin, i;
	if (root == NULL)
		return;
	proot = build_ascii_tree(root);
	compute_edge_lengths(proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
	{
		lprofile[i] = INFINITY;
	}
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
	{
		xmin = MIN(xmin, lprofile[i]);
	}
	for (i = 0; i < proot->height; i++)
	{
		print_next = 0;
		print_level(proot, -xmin, i);
		printf("\n");
	}
	if (proot->height >= MAX_HEIGHT)
	{
		printf("(This tree is taller than %d, and may be drawn incorrectly.)\n",
			   MAX_HEIGHT);
	}
	free_ascii_tree(proot);
}

#endif //LEETCODE_DAILY_H
