#include "leetcode_daily.h"

void solve(int *nums, const int numsSize, int k) {
	(void) nums;
	(void) numsSize;
	(void) k;
}

typedef struct s_input
{
	Array	nums;
	int		k;
	Array	expected;
}	Input;

int ft_do_test(Input *input)
{
	int		check_val;
	Array	nums = input->nums;
	size_t	bytes = nums.size * sizeof(int);

	int (*const result)[nums.size] = memcpy((int *) malloc(bytes),
											nums.arr, bytes);
	solve(*result, nums.size, input->k);

	check_val = (input->expected.size == nums.size);
	if (!check_val)
	{
		printf("resultTotalSize doesn't match expected value\n");
		printf("got array of size [\"%d\"] whilst \"%d\" was to be expected\n",
			   nums.size, input->expected.size);
	}
	else
	{
		check_val = (memcmp(result, input->expected.arr, bytes) == 0);
		if (!check_val)
		{
			printf("got ");
			ft_print_int_tab((int *)(*result), nums.size, " whilst ");
			ft_print_array_eol(&input->expected, "");
			printf(" was to be expected \n");
		}
	}
	check(check_val);
	free(*result);
	return (0);
}

int main(void)
{
	int i;
	Input inputs[] = {
		{
			.nums = (Array) {.arr = (int[7]) {1,2,3,4,5,6,7}, .size = 7},
			.k = 3,
			.expected = (Array) {.arr = (int[7]) {5,6,7,1,2,3,4}, .size = 7}
		},
		{
			.nums = (Array) {.arr = (int[4]) {-1,-100,3,99}, .size = 4},
			.k = 2,
			.expected = (Array) {.arr = (int[4]) {3,99,-1,-100}, .size = 4}
		},
	};
	size_t inputs_size = (sizeof(inputs) / sizeof(inputs[0]));

	signal(SIGSEGV, sigsegv);
	i = -1;
	while (++i < (int) inputs_size)
		ft_do_test(&inputs[i]);
	return (EXIT_SUCCESS);
}