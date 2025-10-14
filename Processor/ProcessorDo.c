#pragma once

#include "Processor.h"

static void DoPush(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);


    Err->stack = StackPush(&(Proc->Stk), Proc->Code[++Proc->InstrPtr]);

    Proc->InstrPtr++;
}

static void DoOut(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);


    long long int Val = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val);

    printf("%lld\n", Val);

    Proc->InstrPtr++;
}

static void DoAdd(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(Proc->Stk), &Val2);

    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(Proc->Stk), Val1 + Val2);
    
    Proc->InstrPtr++;

    //printf("val1=%lld,val2=%lld\n", Val1, Val2);
    //getchar();
}

static void DoSub(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(Proc->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(Proc->Stk), Val2 - Val1);
    
    Proc->InstrPtr++;
}

static void DoMul(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(Proc->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(Proc->Stk), Val1 * Val2);
    
    Proc->InstrPtr++;
}

static void DoDiv(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(Proc->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    if(Val2 == 0)
    {
        printf(colorize("Divizion by zero, aborted\n", _RED_));
        
        return;
    }
    
    Err->stack = StackPush(&(Proc->Stk), Val2 / Val1);
    
    Proc->InstrPtr++;
}

static void DoSqrt(processor *Proc, processor_err_struct_t *Err)
{

    assert(Proc);
    assert(Err);


    long long int Val = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val);

    if(Err->stack)
        return;

    if(Val < 0)
    {
        printf(colorize("SQRT of negative number, aborted\n", _RED_));

        return;
    }

    Err->stack = StackPush(&(Proc->Stk), (long long int)sqrt((double)Val));

    Proc->InstrPtr++;
}

static void DoPushr(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    Err->stack = StackPush(&(Proc->Stk), Proc->Regs[Proc->Code[++Proc->InstrPtr]]);

    Proc->InstrPtr++;
}

static void DoPopr(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    Err->stack = StackPop(&(Proc->Stk), &(Proc->Regs[Proc->Code[++Proc->InstrPtr]]));

    Proc->InstrPtr++;
}


static void DoJmp(processor *Proc, processor_err_struct_t *Err, bool (*Comp) (long long int, long long int))
{
    assert(Proc);
    assert(Err);

    if(Comp == NULL)
    {
        Proc->InstrPtr = (size_t)Proc->Code[Proc->InstrPtr + 1];

        return;
    }

    long long Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(Proc->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(Proc->Stk), &Val2);

    if(Err->stack)
        return;
    
    if(Comp(Val1, Val2))
        Proc->InstrPtr = (size_t)Proc->Code[Proc->InstrPtr + 1];
    
    else
        Proc->InstrPtr += 2;
}

static void DoCall(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    Err->stack = StackPush(&(Proc->RetAddr), (data_t)Proc->InstrPtr + 2);

    DoJmp(Proc, Err, NULL);
}

static void DoRet(processor *Proc, processor_err_struct_t *Err)
{
    assert(Proc);
    assert(Err);

    long long int Addr = 0;

    Err->stack = StackPop(&(Proc->RetAddr), &Addr);

    Proc->InstrPtr = (size_t)Addr;
}