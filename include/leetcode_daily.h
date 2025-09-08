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
#include <unistd.h>

#ifndef USING_ASAN
#  if defined(__has_feature)
#    if __has_feature(address_sanitizer)
#      define USING_ASAN 1
#    endif
#  endif
#endif

#ifndef USING_ASAN
/* GCC and newer Clang define __SANITIZE_ADDRESS__ with -fsanitize=address */
#  ifdef __SANITIZE_ADDRESS__
#    define USING_ASAN 1
#  endif
#endif

#ifndef USING_ASAN
#  define USING_ASAN 0
#endif

typedef struct Array
{
	int *arr;
	int size;
}	Array;

typedef struct TwoDArray
{
	Array	*arrays;
	int		size;
}	TwoDArray;

typedef struct Grid
{
	int	rows;
	int	cols;
	int	*a;    /* rows * cols, row-major */
}	Grid;

typedef struct CharGrid
{
	int		rows;
	int		cols;
	char	*a;    /* rows * cols, row-major */
}	CharGrid;

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

void sigsegv2(int signal)
{
	const char msg[] = FT_CYAN".SIGSEGV"FT_RESET"\n";
	ssize_t n = write(STDERR_FILENO, msg, sizeof(msg) - 1);
	(void)n;

	/* Restore default and re-raise for core dump */
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGSEGV, &sa, NULL);
	raise(SIGSEGV);

	_exit(128 + signal); /* fallback */
}

/**
 * https://geoffrichards.co.uk/blog/2019/09/c-backtrace/
 * https://scaryreasoner.wordpress.com/2007/11/17/using-ld_preload-libraries-and-glibc-backtrace-function-for-debugging/
 * @param signo
 */
extern inline
void segv_handler(int signo)
{
	const char msg[] = FT_CYAN".SIGSEGV"FT_RESET"\n";
	write(STDERR_FILENO, msg, sizeof(msg) - 1);

	pid_t pid = fork();
	if (pid == 0) {
		/* Child: exec crash helper. Pass PID to inspect via /proc. */
		char pidBuf[32];
		int len = 0;
		/* minimal itoa without snprintf to avoid non-signal-safe calls */
		{
			unsigned long p = (unsigned long)getppid();
			char tmp[32];
			int i = 0;
			if (p == 0) { tmp[i++] = '0'; }
			while (p > 0 && i < (int)sizeof(tmp)) {
				tmp[i++] = (char)('0' + (p % 10));
				p /= 10;
			}
			while (i > 0 && len < (int)sizeof(pidBuf)) {
				pidBuf[len++] = tmp[--i];
			}
			if (len == 0) { pidBuf[len++] = '0'; }
			if (len < (int)sizeof(pidBuf)) pidBuf[len] = '\0';
		}
		const char *argv[] = {"/usr/bin/my-crash-dumper", pidBuf, NULL };
		execve(argv[0], (char * const *)argv, __environ); /* environ is OK to reuse */
		_exit(127);
	}

	/* Parent: either wait minimally or just re-raise for core */
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGSEGV, &sa, NULL);
	raise(SIGSEGV);
	_exit(128 + signo);
}

void check(bool succes)
{
	const char *fmt = succes ? FT_GREEN".OK" : FT_RED".KO";
	printf("> %s "FT_RESET"\n", fmt);
}

int compare_strings(const void* a, const void* b) // NOLINT(*-easily-swappable-parameters)
{
	const char* str1 = *(const char**)a;
	const char* str2 = *(const char**)b;
	return strcmp(str1, str2); // Lexicographic comparison
}

static __attribute__((used,__noinline__))
int grid_cmp(const Grid *a, const Grid *b)
{
	if (a->rows != b->rows)
		return (a->rows - b->rows);
	if (a->cols != b->cols)
		return (a->cols - b->cols);
	size_t i = 0;
	size_t n = (size_t) a->rows * (size_t) a->cols;
	while (i < n)
	{
		int da = a->a[i];
		int db = b->a[i];
		if (da != db)
			return (da - db);
		i++;
	}
	return 0;
}

static __attribute__((used,__noinline__))
void grid_print(const Grid *g)
{
	int row = -1;
	while (++row < g->rows)
	{
		int *grid_row = &g->a[(size_t)row * g->cols];

		int coll = -1;
		printf("[");
		while (++coll < g->cols)
		{
			printf("%d", grid_row[coll]);
			if (coll + 1 < g->cols)
				printf(" ");
		}
		printf("]\n");
	}
}

int arraycmp(Array *p, Array *q)
{
	int ret;

	if (!p || !q)
		return (-1);

	ret = p->size - p->size;
	if (!ret)
		ret = memcmp(p->arr, q->arr, p->size * sizeof(int));
	return (ret);
}

int strarraycmp(StringArray *p, StringArray *q)
{
	int ret;

	if (!p || !q)
		return (-1);

	ret = p->size - p->size;
	if (!ret)
	{
		int i = -1;
		while (++i < p->size)
		{
			ret = strcmp(p->arr[i], q->arr[i]);
			if (ret) break;
		}
	}
	return (ret);
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

void ft_print_array_eol(Array *a, const char *eol)
{
	if (a)
		ft_print_int_tab(a->arr, a->size, eol);
}

void ft_print_array(Array *a)
{
	if (a)
		ft_print_int_tab(a->arr, a->size, NULL);
}

void ft_print_int_tab_null(const int tab[], size_t size, int nil, const char *eol) // NOLINT(*-easily-swappable-parameters)
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
	int				i;
	int				rear = 0;
	int				front = 0;
	struct TreeNode	*root = NULL, *current; // NOLINT(*-isolate-declaration)
	struct TreeNode	*queue[MAX_STACK_SIZE];

	if (size == 0 || arr[0] == null)
		return NULL;

	root = new_node(arr[0]);
	queue[rear++] = root;

	i = 0;
	while (i < size) // Start from the second element in the array
	{
		/* Dequeue the front node */
		current = queue[front++]; // NOLINT(*-core.uninitialized.Assign)

		i++;
		if (i < size && arr[i] != null)
		{
			current->left = new_node(arr[i]);
			queue[rear++] = current->left; // Enqueue the left child
		}
		i++;
		if (i < size && arr[i] != null)
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
#define INFINITY (1<<20) // hack? this seems defined somewhere... math.h?

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

ASCIITree *build_ascii_tree_recursive(TreeNode *t)
{
	ASCIITree *node;

	if (t == NULL)
		return NULL;

	node = malloc(sizeof(ASCIITree));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);

	if (node->left != NULL)
		node->left->parent_dir = -1;

	if (node->right != NULL)
		node->right->parent_dir = 1;

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
	int i;
	int is_left;
	if (node == NULL)
		return;
	is_left = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x - ((node->label_length - is_left) / 2));
	if (node->left != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			lprofile[y + i] = MIN(lprofile[y + i], x - i);
	}
	compute_lprofile(node->left, x - node->edge_length - 1,
					 y + node->edge_length + 1);
	compute_lprofile(node->right, x + node->edge_length + 1,
					 y + node->edge_length + 1);
}

void compute_rprofile(ASCIITree *node, int x, int y)
{
	int i;
	int notleft;
	if (node == NULL)
		return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x + ((node->label_length - notleft) / 2));
	if (node->right != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			rprofile[y + i] = MAX(rprofile[y + i], x + i);
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
	int h;
	int hmin;
	int i;
	int delta;

	if (node == NULL) return;

	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else
	{
		if (node->left != NULL) {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		else
			hmin = 0;

		if (node->right != NULL) {
			for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
				lprofile[i] = INFINITY;
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		else
			hmin = 0;

		delta = 4;
		for (i = 0; i < hmin; i++)
			delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);

		// If the node has two children of height 1, then we allow the
		// two leaves to be within 1, instead of 2
		_Bool leftIsOne = node->left != NULL && node->left->height == 1;
		_Bool rightIsOne = node->right != NULL && node->right->height == 1;
		if ((leftIsOne || rightIsOne) && delta > 4)
			delta--;

		node->edge_length = ((delta + 1) / 2) - 1;
	}

	h = 1;
	if (node->left != NULL)
		h = MAX(node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX(node->right->height + node->edge_length + 1, h);
	node->height = h;
}

static inline
void emit_branch(int spaces, const char *glyph)
{
	int i = -1;

	while (++i < spaces)
		printf(" ");
	print_next += i;
	printf("%s", glyph);
	print_next++;
}

// This function prints the given level of the given tree, assuming
// that the node has the given x coordinate.
void print_level(ASCIITree *node, int x, int level)
{
	int i;
	int is_left;
	if (node == NULL)
		return;
	is_left = (node->parent_dir == -1);
	if (level == 0)
	{
		int l_length = node->label_length;
		for (i = 0; i < (x - print_next - ((l_length - is_left) / 2)); i++)
			printf(" ");

		print_next += i;
		printf("%s", node->label);
		print_next += l_length;
	}
	else
	{
		/**
		 * edge_length - how many spaces (columns) separate the parent node’s
		 * text and the point where its child branches (/ and \) start,
		 * aka the length of the “edge” down to each child.
		 */
		int e_length = node->edge_length;
		if (e_length >= level)
		{
			if (node->left != NULL)
				emit_branch(x - print_next - level, "/");

			if (node->right != NULL)
				emit_branch(x - print_next + level, "\\");
		}
		else
		{
			print_level(node->left, x - e_length - 1, level - e_length - 1);
			print_level(node->right, x + e_length + 1, level - e_length - 1);
		}
	}
}

// prints ascii tree for given BinaryTree structure
void print_tree(struct TreeNode *root)
{
	int			i;
	int			xmin;
	ASCIITree	*tree;

	if (root == NULL) return;

	tree = build_ascii_tree(root);
	compute_edge_lengths(tree);
	for (i = 0; i < tree->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;

	compute_lprofile(tree, 0, 0);
	xmin = 0;
	for (i = 0; i < tree->height && i < MAX_HEIGHT; i++)
		xmin = MIN(xmin, lprofile[i]);

	for (i = 0; i < tree->height; i++)
	{
		print_next = 0;
		print_level(tree, -xmin, i);
		printf("\n");
	}
	if (tree->height >= MAX_HEIGHT)
	{
		printf("(This tree is taller than %d, and may be drawn incorrectly.)\n",
			   MAX_HEIGHT);
	}
	free_ascii_tree(tree);
}

#endif //LEETCODE_DAILY_H
