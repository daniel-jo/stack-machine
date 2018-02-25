#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/* here should be implemented the constraint about 32 bits words... */
//typedef _Bool bool;
typedef int32_t word;

typedef struct Stack 
{ 
	int top; 
	int allocated;
	word* memory; 
} Stack;

typedef Stack* stackp;

stackp newStack();
void freeStack(stackp sp);

bool isEmpty(stackp sp);
bool isFull(stackp sp);

void push(stackp sp, word w);
word pop(stackp sp);

word peek(stackp sp);

#endif