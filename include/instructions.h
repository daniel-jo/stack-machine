#ifndef INSTRUCTION_H
#define INSTRUCTION_H

static const size_t OPR_SIZE = 1; // 1 byte
static const size_t OPD_SIZE = 4; // 4 bytes
static const size_t INST_SIZE = 5; // 5 bytes

// Instruction set of the virtual machine
typedef enum OPR
{
	NOP = 0,
	PUSH,
	ADD,
	SUB,
	OPR_COUNT
} opr_t;

#define OPR_LENGTH 5 // 5 chars (including null terminator)
extern const char OPR_STR[OPR_COUNT][OPR_LENGTH];

char* oprToStr(opr_t opr);

opr_t strToOpr(char* str);

#endif