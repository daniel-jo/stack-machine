#include <string.h>

#include "instructions.h"

const char OPR_STR[OPR_COUNT][OPR_LENGTH] = 
{
	"nop",
	"push",
	"add",
	"sub"
};

char* oprToStr(opr_t opr) { return (char*) OPR_STR[opr]; }

opr_t strToOpr(char* str) 
{
	for ( int i = 0; i < OPR_COUNT; ++i )
	{
		if ( !strcmp(str, OPR_STR[i]) )
		{
			return (opr_t) i;
		}
	}

	return NOP;
}