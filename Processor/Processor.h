#pragma once

#include "../Stack/Stack.h"
#include <sys/stat.h>
#include <stdbool.h>
#include "ColorPrinting.h"
#include "Common.h"
#include "GraphicalInterface.h"
//#include "ProcessorDo.h"


//#define DEBUG_MODE
static const unsigned int RAM_SIZE = 1000;
static const unsigned int VRAM_SIZE = 100 * 100;


typedef enum
{
    UNDEF_REG=-1,
    rax,
    rcx,
    rdx,
    rbx,
    rsp,
    rbp,
    rsi,
    rdi,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,


    REG_COUNT
} regs;

static const size_t NUM_OF_REGS = REG_COUNT;

//static const size_t _NUM_OF_REGS = 16;

typedef struct
{
    size_t _NumOfInstr;

    size_t InstrPtr;

    long long int Regs[16];//gcc
    
    stack_t Stk;

    stack_t RetAddr;

    long long int *Code;

    long long int *RAM;

    long long int *VRAM;

} processor;

typedef enum
{
    PROC_NO_ERR = 0,
    NULLPTR = 50,
    CODE_NULLPTR,
    WRONG_REGS,
    WRONG_FILE,
    WRONG_IP,
    RAM_NULLPTR,
    VRAM_NULLPTR,
    DIV_BY_ZERO,
    SQRT_OF_NEGATIVE,
    RAM_SEG_FAULT,//hz
    VRAM_SEG_FAULT,
    CONSOLE_ERR,

    PROC_UNDEFINED_ERR

} processor_err_t;


typedef struct
{
    stack_err_t stack;

    processor_err_t proc;

} processor_err_struct_t;
/*
static void DoPush(processor *SPU, processor_err_struct_t *Err);
static void DoOut(processor *SPU, processor_err_struct_t *Err);
static void DoAdd(processor *SPU, processor_err_struct_t *Err);
static void DoSub(processor *SPU, processor_err_struct_t *Err);
static void DoMul(processor *SPU, processor_err_struct_t *Err);
static void DoDiv(processor *SPU, processor_err_struct_t *Err);
static void DoSqrt(processor *SPU, processor_err_struct_t *Err);
static void DoPushr(processor *SPU, processor_err_struct_t *Err);
static void DoPopr(processor *SPU, processor_err_struct_t *Err);
static void DoJmp(processor *SPU, processor_err_struct_t *Err,
                    bool (*Comp)(long long int, long long int));
static void DoCall(processor *SPU, processor_err_struct_t *Err);
static void DoRet(processor *SPU, processor_err_struct_t *Err);
*/
bool ProcessorVerifyExt(processor *SPU, processor_err_struct_t *Err);
bool ProcessorInitExt(processor *SPU, const char *BinFilePath, processor_err_struct_t *Err);
bool ProcessorVerify(processor *SPU);
bool ProcessorInit(processor *SPU, const char *BinFilePath);
bool _ProcessorDump(processor *SPU, processor_err_struct_t *Err, const char *File, const unsigned int Line);
void ProcessorDestroy(processor *SPU);
bool LaunchProgram(processor *SPU, processor_err_struct_t *Err);

bool Below(long long int Val1, long long int Val2);
bool BelowOrEqual(long long int Val1, long long int Val2);
bool Above(long long int Val1, long long int Val2);
bool AboveOrEqual(long long int Val1, long long int Val2);
bool Equal(long long int Val1, long long int Val2);
bool NotEqual(long long int Val1, long long int Val2);

#define ProcessorDump(PtrToProc, PtrToErrStrct) \
    _ProcessorDump(PtrToProc, PtrToErrStrct, __FILE__, __LINE__)
