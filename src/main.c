#include "machine.h"

#include <stdio.h>

#define PREFIX '-'
#define NR_OF_OPTS 2
#define DESC_LENGTH 64

const char OPT[NR_OF_OPTS] = 
{
	'h',
	'i'
};

const char OPT_DESC[NR_OF_OPTS][DESC_LENGTH] = 
{
	"Help",
	"Interactive mode"
};

enum Option
{
	OPT_HELP = 'h',
	OPT_INTERACTIVE = 'i'
};

void printOptDesc()
{
	for ( int opt = 0; opt < NR_OF_OPTS; ++opt )
	{
		printf("%c%c\n    %s\n\n", PREFIX, OPT[opt], OPT_DESC[opt]);
	}
}

int main(int argc, char *argv[])
{
	for ( int arg = 1; arg < argc; ++arg )
	{
		if ( argv[arg][0] != PREFIX )
		{
			execFile(argv[arg]);
			break;
		}
		
		switch ( (int)argv[arg][1] )
		{
			case OPT_HELP:
				printOptDesc();
				break;
				
			case OPT_INTERACTIVE:
				// TODO: run machine in interactive mode
				break;
		}
	}
}
