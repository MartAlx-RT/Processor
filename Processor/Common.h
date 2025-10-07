#pragma once

#include <stddef.h>


extern const char *COMMANDS[];
extern const size_t COMMANDS_SIZE;

extern const char *REGS_NAME[];
extern const size_t NUM_OF_REGS;

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

    _SKIP_LINE,
    _UNDEF

} instruction;

