#include "stack.h"
#include "machine.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NR_OF_INSTRUCTIONS 3
#define INSTRUCTION_LENGTH 16

// Instruction set of the virtual machine
enum Instruction
{
    instPush,
    instAdd,
    instSub
};

// For assembler
const char INSTRUCTION[NR_OF_INSTRUCTIONS][INSTRUCTION_LENGTH] = 
{
	"push",
	"add",
	"sub"
};

// Loads file in to memory and separates operator/instruction from operand.
char** loadFile(const char* fileName, size_t* opsCount)
{
	int linesAlloc = 64;
	int maxLineLen = 64;

    // Allocate lines of text.
	char** ops = (char**)malloc(sizeof(char*)*linesAlloc);
	if ( ops == NULL )
    {
    	fprintf(stderr, "Out of memory (1).\n");
		exit(1);
	}

	FILE *fp = fopen(fileName, "r");
	if ( fp == NULL )
	{
		fprintf(stderr, "Error opening file.\n");
		exit(2);
	}

	int i = 0;
	do
	{
		if ( i >= linesAlloc )
		{
			int newLinesAlloc;

			// Double our allocation and re-allocate.
			newLinesAlloc = linesAlloc*2;
			ops = (char **)realloc(ops,sizeof(char*) * newLinesAlloc);
			if ( ops == NULL )
			{
				fprintf(stderr, "Out of memory.\n");
				exit(3);
			}
			linesAlloc = newLinesAlloc;
		}

		// Allocate space for the next line.
		ops[i] = (char*)malloc(maxLineLen);
		if ( ops[i] == NULL )
        {
        	fprintf(stderr, "Out of memory (3).\n");
        	exit(4);
        }

		// Get the first character in the line.
		char currChar = fgetc(fp);
		int j = 0;
		if ( feof(fp) ) 
		{
			break;
		} 
		
        // Concatenate the character if it's not a space or new line (\n, \r).
		while ( currChar != ' ' && currChar != '\n' && currChar != '\r' )
		{
			ops[i][j++] = currChar;
			currChar = fgetc(fp);
		}
		ops[i++][j] = '\0';
	}
	while ( true );
	
    fclose(fp);

	*opsCount = i;
	return ops;   
}


// Assembles the content of the file to bytecode.
int32_t* assemble(char** ops, size_t* opsCount)
{ 
	word* bytecode = (word*)malloc(sizeof(word) * (*opsCount));
	for ( int i = 0; i < *opsCount; ++i )
	{
		char c = ops[i][0];

		if ( isalpha(c) ) 
		{
			// Match instruction with the instruction set and convert it to bytecode.
			for ( int instructionNr = 0; instructionNr < NR_OF_INSTRUCTIONS; ++instructionNr )
			{
				if ( !strcmp(ops[i], INSTRUCTION[instructionNr]) )
				{
					bytecode[i] = instructionNr;									
				}
			}
		}
		else
		{
			// Convert number represented by characters to integer.
			int number;
			int count;
			int result = sscanf(ops[i], "%d%n", &number, &count);
			
			if ( result != 0 || count != sizeof(ops[i]) )
			{
				bytecode[i] = number;
			}
		}		
	}

	return bytecode;
}

// Interprets the bytecode,
void execFile(const char* fileName)
{
	size_t* opsCount = (size_t*) malloc(sizeof(size_t));
	
	char** ops = loadFile(fileName, opsCount);
	
	word* bytecode = assemble(ops, opsCount);


	for ( int i = *opsCount; i >= 0; --i )
		free(ops[i]);
	free(ops);
	
	stackp sp = newStack();
	
	for ( int i = 0; i < *opsCount; ++i )
	{
		switch (bytecode[i])
		{
			case instPush:
				push(sp, bytecode[++i]);
				break;

			case instAdd:
			{
				word sum = pop(sp) + pop(sp);
				push(sp, sum);
				break;
			}
						
			case instSub:
			{
				word diff = pop(sp) - pop(sp);
				push(sp, diff);
				break;
			}
		}
	}

	printf("result: %d\n", pop(sp));

	free(opsCount);	
	free(bytecode);
	freeStack(sp);
}
