#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct Stack 
{ 
	int top; 
	int allocated;
	int32_t* memory; 
} Stack;

typedef Stack* stackp;

stackp newStack();
void freeStack(stackp sp);

void push(stackp sp, int32_t data);
int32_t pop(stackp sp);

int32_t peek(stackp sp);

#endif
