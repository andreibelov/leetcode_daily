/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:01:11 by abelov            #+#    #+#             */
/*   Updated: 2025/04/24 14:01:12 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * 189. Rotate Array
 *
 * Given an integer array nums, rotate the array to the right by k steps,
 * where k is non-negative.
 *
 * @param nums
 * @param numsSize
 * @param k
 */
void rotate(int *nums, int numsSize, int k);


static inline __attribute__((always_inline))
void	ft_swap(int *a, int *b)
{
	register const int	temp = *b;

	*b = *a;
	*a = temp;
}

void	rev_int_tab(int *tab, int size)
{
	int	*head;
	int	*tail;

	head = tab;
	tail = tab + size - 1;
	while (head <= tail)
		ft_swap(head++, tail--);
}

void rotate(int *nums, int numsSize, int k)
{
	k %= numsSize;
	rev_int_tab(nums , numsSize);
	rev_int_tab(nums , k);
	rev_int_tab(nums + k, numsSize - k);
}