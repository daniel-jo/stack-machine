#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

stackp newStack()
{
	stackp sp = (stackp) malloc(sizeof(Stack));
	sp->top = -1;
	sp->allocated = 1024;
	sp->memory = (int32_t*) malloc(sizeof(int32_t) * sp->allocated);

	return sp;
}

void freeStack(stackp sp)
{
	free(sp->memory);
	free(sp);
}

bool isEmpty(stackp sp)
{
	if ( sp->top < 0 )
		return true;
		
	return false;	
}

bool isFull(stackp sp)
{
	if ( sp->top >= sp->allocated - 1 )
		return true;
		
	return false;
}

void reallocMem(stackp sp)
{
	int newSize = sp->allocated*2;
	sp->memory = (int32_t*) realloc(sp->memory, sizeof(int32_t) * newSize);
	if ( sp->memory == NULL )
	{
		fprintf(stderr, "Out of memory.\n");
		exit(-1);
	}
	sp->allocated = newSize;
}

void push(stackp sp, int32_t data)
{
	if ( isFull(sp) )
		reallocMem(sp);

	sp->memory[++sp->top] = data;
}

int32_t pop(stackp sp)
{
	if ( isEmpty(sp) )
	{
		fprintf(stderr, "Could not retrieve data, the stack is empty! \n");
		exit(-1);	
	}

	return sp->memory[sp->top--];
}

int32_t peek(stackp sp)
{
	if ( isEmpty(sp) )
	{
		fprintf(stderr, "Could not retrieve data, the stack is empty! \n");
		exit(-1);	
	}

	return sp->memory[sp->top];
}
