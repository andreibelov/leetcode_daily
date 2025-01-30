/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myCalendar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 00:16:43 by abelov            #+#    #+#             */
/*   Updated: 2024/09/27 00:16:44 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#define MAX_STACK_SIZE 10000

typedef enum bool
{
	false = 0,
	true = 1
} bool;

typedef struct Booking
{
	int start, end;
	struct Booking *left;
	struct Booking *right;
} Booking;

/**
 * 729. My Calendar I
 *
 * You are implementing a program to use as your calendar.
 * We can add a new event if adding the event will not cause a double booking.
 *
 * Your MyCalendar struct will be instantiated and called as such:
 * MyCalendar* obj = myCalendarCreate();
 * bool param_1 = myCalendarBook(obj, start, end);
 * myCalendarFree(obj);
*/
typedef struct
{
	Booking *root;
} MyCalendar;

typedef struct
{
	Booking **ptr;
	int start;
	int end;
}	StackItem;

MyCalendar *myCalendarCreate()
{
	MyCalendar *calendar = (MyCalendar *)calloc(1, sizeof(MyCalendar));
	return calendar;
}

static bool book(Booking **ptr, int start, int end)
{
	int sp = 0;
	StackItem stack[MAX_STACK_SIZE];

	Booking **currentPtr;
	Booking *new;

	// Push the initial root node onto the stack
	stack[sp++] = (StackItem) {ptr, start, end};

	while (sp)
	{
		StackItem current = stack[--sp];
		currentPtr = current.ptr;
		if (!*currentPtr)
		{
			// Found a null spot, insert new booking here
			new = (Booking *)malloc(sizeof(Booking));
			*new = (Booking) {current.start, current.end, 0, 0};
			*currentPtr = new;
			return true;
		}
		else
		{
			// Traverse to the left or right depending on the booking times
			if (current.end <= (*currentPtr)->start)
			{
				// Push left node onto the stack
				stack[sp++] = (StackItem) {&(*currentPtr)->left, current.start, current.end};
			}
			else if ((*currentPtr)->end <= current.start)
			{
				// Push right node onto the stack
				stack[sp++] = (StackItem) {&(*currentPtr)->right, current.start, current.end};
			}
			else
			{
				// Overlap detected, booking cannot be made
				return false;
			}
		}
	}

	return false;
}
bool myCalendarBook(MyCalendar *obj, int start, int end)
{
	return book(&obj->root, start, end);
}

static void freeBooking(Booking *ptr)
{
	if (ptr)
	{
		freeBooking(ptr->left);
		freeBooking(ptr->right);
		free(ptr);
	}
}

void myCalendarFree(MyCalendar *obj)
{
	freeBooking(obj->root);
	free(obj);
}
