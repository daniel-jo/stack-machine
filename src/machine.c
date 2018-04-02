#include <stdint.h>
#include <stdio.h>

#include "machine.h"
#include "assembler.h"
#include "instructions.h"
#include "stack.h"

uint32_t convertToUint32(uint8_t* a4) 
{
	return a4[0] | (a4[1] << 8) | (a4[2] << 16) | (a4[3] << 24);
}

void instPush(stackp sp, uint32_t data) 
{
	push(sp, data);
}

void instAdd(stackp sp) 
{
	push(sp, pop(sp) + pop(sp));
}

void instSub(stackp sp)
{
	push(sp, pop(sp) - pop(sp));
}

void instMult(stackp sp)
{
	push(sp, pop(sp) * pop(sp));
}

void instDiv(stackp sp)
{
	push(sp, pop(sp) / pop(sp));
}

// Interprets the bytecode,
void execFile(const char* fileName)
{
	size_t instCount = 0;

	uint8_t** instructions = assembleFile(fileName, &instCount);

	stackp sp = newStack();

	size_t pc = 0;
	while ( pc < instCount )
	{
		uint8_t* instruction = instructions[pc];

		switch (instruction[0])
		{
			default:
				printf("Unknown instruction");
				break;

			case PUSH:
				instPush(sp, convertToUint32(++instruction));
				break;

			case ADD:
				instAdd(sp);
				break;

			case SUB:
				instSub(sp);
				break;
			
			case MULT:
				instMult(sp);
				break;

			case DIV:
				instDiv(sp);
				break;
		}

		++pc;
	}

	printf("result: %d\n", pop(sp));

	for ( int i = instCount; i >= 0; --i )
		free(instructions[i]);
	free(instructions);	
	freeStack(sp);
}
