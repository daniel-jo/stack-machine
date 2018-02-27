#include "stack.h"

#include <stdio.h>

stackp newStack()
{
	stackp sp = (stackp) malloc(sizeof(Stack));
	sp->top = -1;
	sp->allocated = 1024;
	sp->memory = (word*) malloc(sizeof(word) * sp->allocated);
	
	return sp;
}

void freeStack(stackp sp)
{
	free(sp->memory);
	free(sp);
}

bool isEmpty(stackp sp)
{
	if ( sp->top == -1 )
		return true;
	else
		return false;	
}

bool isFull(stackp sp)
{
	if ( sp->top == sp->allocated - 1 )
		return true;
	else
		return false;
}

void push(stackp sp, word w)
{
	if ( !isFull(sp) )
	{
		++sp->top;
		sp->memory[sp->top] = w;
	}
	else
	{
		int newSize = sp->allocated*2;
		sp->memory = (word*) realloc(sp->memory, sizeof(word) * newSize);
		if ( sp->memory == NULL )
		{
			fprintf(stderr, "Out of memory.\n");
			exit(3);
		}
		sp->allocated = newSize;
	}
}

word pop(stackp sp)
{
	word w = -1;
	
	if ( !isEmpty(sp) ) 
	{
		w = sp->memory[sp->top];
		--sp->top;
	}
	else
		printf("Could not retrieve data, the stack is empty! \n");	
		
	return w;
}

word peek(stackp sp)
{
	if ( !isEmpty(sp) )
		return sp->memory[sp->top];
	else
		printf("Could not retrieve data, the stack is empty! \n");	
}
