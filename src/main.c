#include "stack.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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
//const int NR_OF_INSTRUCTIONS = 3;
//const int INSTRUCTION_LENGTH = 16; //Instruction length in byte (how many chars) Needed?
const char INSTRUCTION[NR_OF_INSTRUCTIONS][INSTRUCTION_LENGTH] = 
{
	"push",
	"add",
	"sub"
};

// Loads file in to memory and separates operator/instruction from operand
char** loadFile(const char* fileName, size_t* opsCount)
{
	int lines_allocated = 64;
	int max_line_len = 100;

    // Allocate lines of text
	char** ops = (char**)malloc(sizeof(char*)*lines_allocated);
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
		// Have we gone over our line allocation?
		if ( i >= lines_allocated )
		{
			int new_size;

			// Double our allocation and re-allocate
			new_size = lines_allocated*2;
			ops = (char **)realloc(ops,sizeof(char*)*new_size);
			if ( ops == NULL )
			{
				fprintf(stderr, "Out of memory.\n");
				exit(3);
			}
			lines_allocated = new_size;
		}

		// Allocate space for the next line
		ops[i] = (char*)malloc(max_line_len);
		if ( ops[i] == NULL )
        {
        	fprintf(stderr, "Out of memory (3).\n");
        	exit(4);
        }

		// Get the next character
		char _char = fgetc(fp);	
		if ( feof(fp) ) 
		{
			break;
		} 
		
        // Concatenate the character if it's not an space or new line (\n, \r)
		int j = 0;
		for ( ; _char != ' ' && _char != '\n' && _char != '\r'; ++j )
		{		
			ops[i][j] = _char;
			_char = fgetc(fp);
		}
		ops[i][j] = '\0';

		++i;
	}
	while (1);
	
    // Close file
    fclose(fp);

	*opsCount = i;

	return ops;   
}


// Compiles the content of the file to bytecode
int32_t* compile(const char* fileName, size_t* opsCount)
{ 
	char** ops = loadFile(fileName, opsCount);
	
	//int32_t* bytecode = convertToBytecode(ops, opsCount);
	int32_t* bytecode = (int32_t*)malloc(sizeof(int32_t) * (*opsCount)); //int32_t bytecode[*opsCount];
	for ( int i = 0; i < *opsCount; ++i )
	{
		char c = ops[i][0];

		if ( isalpha(c) ) 
		{
			// Match instruction with the instruction set and convert it to bytecode
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
			// Convert number represented by characters to integer
			int number;
			int count;
			int result = sscanf(ops[i], "%d%n", &number, &count);
			
			if (result != 0 || count != sizeof(ops[i]))
			{
				bytecode[i] = number;
			}
		}		
	}
				
	// Free memory
	for ( int i = *opsCount; i >= 0; i-- )
		free(ops[i]);
	free(ops);

	return bytecode;
}

// Interprets the bytecode
void execBytecode(int32_t* bytecode, size_t* opsCount)
{
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
	
	freeStack(sp);
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
	    const char *fileName = argv[1]; // should check that argc > 1
		size_t* opsCount = (size_t*) malloc(sizeof(size_t));
	
		int32_t* bytecode = compile(fileName, opsCount);
	
		execBytecode(bytecode, opsCount);
	  	
		free(opsCount);	
		free(bytecode);
	}
}
