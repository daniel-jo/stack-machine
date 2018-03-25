#include "stack.h"
#include "machine.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <limits.h>

#define NR_OF_OPR 3
#define OPR_LENGTH (32 / CHAR_BIT) + 1 // = 5

#define OPR_SIZE 1 // 1 byte
#define OPD_SIZE 4 // 4 bytes
#define INST_SIZE OPR_SIZE + OPD_SIZE

// Instruction set of the virtual machine
enum Instruction
{
	instPush,
	instAdd,
	instSub
};

// For assembler
const char OPERATOR[NR_OF_OPR][OPR_LENGTH] = 
{
	"push",
	"add",
	"sub",
};

void extractInst(char* line, char SEPARATOR, char* opr, char** opd)
{
	char* spearatorPos = strchr(line, SEPARATOR);
	if ( spearatorPos )
	{
		size_t len = spearatorPos-line;
		strncpy(opr, line, len);
		opr[len] = '\0';

		*opd = spearatorPos;
		char* endl = strchr(*opd, '\n');
		endl[0] = '\0';
	}
	else
	{
		memcpy(opr, line, sizeof &opr);
		char* endl = strchr(opr, '\n');
		endl[0] = '\0';
		*opd = NULL;
	}
}

uint8_t* assembleInst(char* opr, char** opd)
{
	uint8_t* instruction = (uint8_t*) malloc(INST_SIZE);
	if ( instruction == NULL )
	{
		fprintf(stderr, "Out of memory (3).\n");
		exit(4);
	}

	for ( int i = 0; i < NR_OF_OPR; ++i )
	{
		if ( !strcmp(opr, OPERATOR[i]) )
		{
			instruction[0] = i;
			break;
		}
	}

	// If operand is a number convert it to integer.
	if ( *opd != NULL && !isalpha(*opd[0]) ) 
	{
		int32_t number;
		int count;
		int result = sscanf(*opd, "%d%n", &number, &count);

		if ( result != 0 || count != sizeof(*opd) )
		{
			instruction[1] = number;
			instruction[2] = number >> 8;
			instruction[3] = number >> 16;
			instruction[4] = number >> 24;
		}
	}
	else
	{
		// TODO: Handle jump label.
	}

	return instruction;
}

// Loads each line of the file into memory.
uint8_t** assembleFile(const char* fileName, size_t* instCount)
{
	uint8_t** instructions = (uint8_t**)malloc(sizeof(uint8_t*)*32);

	if ( !instructions )
	{
		fprintf(stderr, "Out of memory (1).\n");
		exit(1);
	}

	int maxLineLen = 64;
	char line[maxLineLen];
	const char SEPARATOR = ' ';

	char opr[OPR_LENGTH];
	char* opd = NULL;

	FILE* fp = fopen(fileName, "r");
	if ( !fp )
	{
		fprintf(stderr, "Error opening file.\n");
		exit(2);
	}

	while ( true )
	{ 
		if ( !fgets(line, maxLineLen, fp) )
			break;
		extractInst(line, SEPARATOR, opr, &opd);

		instructions[*instCount] = assembleInst(opr, &opd);

		memset(opr, 0, sizeof &opr);

		(*instCount)++;
	}

	fclose(fp);

	return instructions;
}

uint32_t convertToUint32(uint8_t* a4) {
	return a4[0] | (a4[1] << 8) | (a4[2] << 16) | (a4[3] << 24);
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
			case instPush:
				push(sp, convertToUint32(++instruction));
				break;

			case instAdd:
			{
				uint32_t sum = pop(sp) + pop(sp);
				push(sp, sum);
				break;
			}

			case instSub:
			{
				uint32_t diff = pop(sp) - pop(sp);
				push(sp, diff);
				break;
			}
		}

		++pc;
	}

	printf("result: %d\n", pop(sp));

	for ( int i = instCount; i >= 0; --i )
		free(instructions[i]);
	free(instructions);	
	freeStack(sp);
}
