/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myCalendarTwo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 19:28:02 by abelov            #+#    #+#             */
/*   Updated: 2024/09/27 19:28:03 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#define MAX_CAPACITY 1001

typedef enum bool
{
	false = 0,
	true = 1
} bool;

typedef struct
{
	int start;
	int end;
} Event;

typedef struct
{
	Event *events;
	int size;
	int capacity;
} MyCalendarTwo;

/**
 * 731. My Calendar II
 *
 * You are implementing a program to use as your calendar.
 * We can add a new event if adding the event will not cause a triple booking.
 *
 * A triple booking happens when three events have some non-empty intersection
 * (i.e., some moment is common to all the three events.).
 */
MyCalendarTwo *myCalendarTwoCreate()
{
	MyCalendarTwo *obj = (MyCalendarTwo *) malloc(sizeof(MyCalendarTwo));
	obj->events = (Event *) malloc(sizeof(Event) * MAX_CAPACITY);
	obj->size = 0;
	obj->capacity = MAX_CAPACITY;
	return obj;
}

bool myCalendarTwoBook(MyCalendarTwo *obj, int start, int end)
{
	// Count for overlaps and double overlaps
	int overlaps[MAX_CAPACITY][2]; // Using a fixed size array for simplicity
	int overlapCount = 0;

	// Check existing events for overlaps
	for (int i = 0; i < obj->size; i++)
	{
		if (start < obj->events[i].end && end > obj->events[i].start)
		{
			overlaps[overlapCount][0] = obj->events[i].start > start ? obj->events[i].start : start;
			overlaps[overlapCount][1] = obj->events[i].end < end ? obj->events[i].end : end;
			overlapCount++;
		}
	}

	// Check for triple booking among the overlaps
	for (int i = 0; i < overlapCount; i++)
	{
		for (int j = i + 1; j < overlapCount; j++)
		{
			if (!(overlaps[i][1] <= overlaps[j][0] || overlaps[i][0] >= overlaps[j][1]))
			{
				// Triple booking found
				return false;
			}
		}
	}

	// Add new event
	obj->events[obj->size].start = start;
	obj->events[obj->size].end = end;
	obj->size++;

	return true;
}

void myCalendarTwoFree(MyCalendarTwo *obj)
{
	free(obj->events);
	free(obj);
}
