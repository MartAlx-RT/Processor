#pragma once

#include <stddef.h>


extern const char *COMMANDS[];
extern const size_t COMMANDS_SIZE;

extern const char *REGS_NAME[];
//static const size_t NUM_OF_REGS = 16;

extern const unsigned int LABLES_SIZE;

typedef struct
{
    char Name[50];//gcc

    long long int Loc;

    unsigned int LocInLine;

} label;

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
    PUSHM,
    POPM,
    PUSHV,
    POPV,
    DRW,
    PAUSE,

    _SKIP_LINE,
    _UNDEF

} instruction;

