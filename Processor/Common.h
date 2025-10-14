#pragma once

#include <stddef.h>


extern const char *COMMANDS[];
extern const size_t COMMANDS_SIZE;

extern const char *REGS_NAME[];
static const size_t NUM_OF_REGS = 16;

extern const int LABLES_SIZE;
extern int Lables[1000];

typedef enum
{
    PUSH,
    OUT,
    HLT,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    PUSHR,
    POPR,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
    CALL,
    RET,

    _SKIP_LINE,
    _UNDEF

} instruction;

