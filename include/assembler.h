#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdlib.h>

uint8_t* assembleInst(char* opr, char** opd);

uint8_t** assembleFile(const char* fileName, size_t* instCount);

#endif