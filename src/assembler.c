#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "instructions.h"

#define checkOutOfMem(data) \
({ \
	if ( !data ) \
	{ \
		fprintf(stderr, "Out of memory.\n"); \
		exit(EXIT_FAILURE); \
	} \
})

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
	if ( !*opd )
	{
		uint8_t* instruction = (uint8_t*) malloc(OPR_SIZE); 
		checkOutOfMem(instruction);
		*instruction = strToOpr(opr);
		return instruction;
	}
	
	uint8_t* instruction = (uint8_t*) malloc(INST_SIZE);

	checkOutOfMem(instruction);

	instruction[0] = strToOpr(opr);

	// If operand is a number convert it to integer.
	if ( !isalpha(*opd[0]) ) 
	{
		int32_t number;
		int count;
		int result = sscanf(*opd, "%d%n", &number, &count);

		if ( result != 0 || count != sizeof(*opd) )
		{
			instruction[1] = number & 0x000000ff;
			instruction[2] = (number & 0x0000ff00) >> 8;
			instruction[3] = (number & 0x00ff0000) >> 16;
			instruction[4] = (number & 0xff000000) >> 24;
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

	checkOutOfMem(instructions);

	int maxLineLen = 64;
	char line[maxLineLen];
	const char SEPARATOR = ' ';

	char opr[OPR_LENGTH];
	char* opd = NULL;

	FILE* fp = fopen(fileName, "r");
	if ( !fp )
	{
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
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