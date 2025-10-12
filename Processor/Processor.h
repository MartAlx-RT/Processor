#pragma once

#include "../Stack/Stack.h"
#include <sys/stat.h>
#include <stdbool.h>
#include "ColorPrinting.h"
#include "Common.h"

static const size_t _NUM_OF_REGS = 16;



typedef struct
{
    size_t _NumOfInstr;

    size_t InstrPtr;

    long long int Regs[NUM_OF_REGS];
    
    stack_t Stk;
    
    long long int *Code;

} processor;

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
    r15

} regs;

// static const char *REGS_NAME[] =
//     {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

typedef enum
{
    PROC_NO_ERR = 0,
    NULLPTR = 50,
    CODE_NULLPTR,
    WRONG_REGS,
    WRONG_FILE,
    WRONG_IP,
    PROC_UNDEFINED_ERR

} processor_err_t;


typedef struct
{
    stack_err_t stack;

    processor_err_t proc;

} processor_err_struct_t;


bool ProcessorVerifyExt(processor *Proc, processor_err_struct_t *ProcErr);

bool ProcessorInitExt(processor *Proc, const char *BinFilePath, processor_err_struct_t *ProcErr);

bool ProcessorVerify(processor *Proc);

bool ProcessorInit(processor *Proc, const char *BinFilePath);

bool _ProcessorDump(processor *Proc, processor_err_struct_t *Err, const char *File, const unsigned int Line);


#define ProcessorDump(PtrToProc, PtrToErrStrct)\
 _ProcessorDump(PtrToProc,PtrToErrStrct,__FILE__,__LINE__)

void ProcessorDestroy(processor *Proc);

bool LaunchProgram(processor *Proc, processor_err_struct_t *Err);

