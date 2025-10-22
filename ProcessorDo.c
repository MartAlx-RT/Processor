//#pragma once
//DONT FORGET TO ADD ++IP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "Processor.h"

static void DoPush(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);


    Err->stack = StackPush(&(SPU->Stk), SPU->Code[++SPU->InstrPtr]);

    SPU->InstrPtr++;
}

static void DoOut(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);


    long long int Val = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val);

    printf("%lld\n", Val);

    SPU->InstrPtr++;
}

static void DoAdd(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(SPU->Stk), &Val2);

    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(SPU->Stk), Val1 + Val2);
    
    SPU->InstrPtr++;

    //printf("val1=%lld,val2=%lld\n", Val1, Val2);
    //getchar();
}

static void DoSub(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(SPU->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(SPU->Stk), Val2 - Val1);
    
    SPU->InstrPtr++;
}

static void DoMul(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(SPU->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    Err->stack = StackPush(&(SPU->Stk), Val1 * Val2);
    
    SPU->InstrPtr++;
}

static void DoDiv(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    long long int Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(SPU->Stk), &Val2);
    
    if(Err->stack)
        return;
    
    if(Val1 == 0)
    {
        printf(colorize("Divizion by zero, aborted\n", _RED_));

        Err->proc = DIV_BY_ZERO;

        return;
    }
    
    Err->stack = StackPush(&(SPU->Stk), Val2 / Val1);
    
    SPU->InstrPtr++;
}

static void DoSqrt(processor *SPU, processor_err_struct_t *Err)
{

    assert(SPU);
    assert(Err);


    long long int Val = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val);

    if(Err->stack)
        return;

    if(Val < 0)
    {
        printf(colorize("SQRT of negative number, aborted\n", _RED_));

        Err->proc = SQRT_OF_NEGATIVE;

        return;
    }

    Err->stack = StackPush(&(SPU->Stk), (long long int)sqrt((double)Val));

    SPU->InstrPtr++;
}

static void DoPushr(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    if(SPU->Code[SPU->InstrPtr + 1] >= (long long int)NUM_OF_REGS || SPU->Code[SPU->InstrPtr + 1] < 0)
    {
        Err->proc = WRONG_REGS;

        printf(colorize("Wrong bytecode, aborted\n", _RED_));

        return;
    }

    Err->stack = StackPush(&(SPU->Stk), SPU->Regs[SPU->Code[++SPU->InstrPtr]]);

    SPU->InstrPtr++;
}

static void DoPopr(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    if(SPU->Code[SPU->InstrPtr + 1] >= (long long int)NUM_OF_REGS || SPU->Code[SPU->InstrPtr + 1] < 0)
    {
        Err->proc = WRONG_REGS;

        printf(colorize("Wrong bytecode, aborted\n", _RED_));

        return;
    }

    Err->stack = StackPop(&(SPU->Stk), &(SPU->Regs[SPU->Code[++SPU->InstrPtr]]));

    SPU->InstrPtr++;
}


static void DoJmp(processor *SPU, processor_err_struct_t *Err, bool (*Comp) (long long int, long long int))
{
    assert(SPU);
    assert(Err);

    if(Comp == NULL)
    {
        SPU->InstrPtr = (size_t)SPU->Code[SPU->InstrPtr + 1];

        return;
    }

    long long Val1 = 0, Val2 = 0;

    Err->stack = StackPop(&(SPU->Stk), &Val1);

    if(Err->stack)
        return;
    
    Err->stack = StackPop(&(SPU->Stk), &Val2);

    if(Err->stack)
        return;
    
    if(Comp(Val1, Val2))
        SPU->InstrPtr = (size_t)SPU->Code[SPU->InstrPtr + 1];
    
    else
        SPU->InstrPtr += 2;
}

static void DoCall(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    Err->stack = StackPush(&(SPU->RetAddr), (data_t)SPU->InstrPtr + 2);

    DoJmp(SPU, Err, NULL);
}

static void DoRet(processor *SPU, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);

    long long int Addr = 0;

    Err->stack = StackPop(&(SPU->RetAddr), &Addr);

    SPU->InstrPtr = (size_t)Addr;
}

static void DoPushm(processor *SPU, long long int *Mem, const unsigned int MemSize, 
                    const processor_err_t SegFaultErr, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);
    assert(Mem);

    if(SPU->Code[SPU->InstrPtr + 1] >= (long long int)NUM_OF_REGS || SPU->Code[SPU->InstrPtr + 1] < 0)
    {
        Err->proc = WRONG_REGS;

        printf(colorize("Wrong bytecode, aborted\n", _RED_));

        return;
    }

    if(SPU->Regs[SPU->Code[SPU->InstrPtr + 1]] >= MemSize || SPU->Regs[SPU->Code[SPU->InstrPtr + 1]] < 0)
    {
        Err->proc = SegFaultErr;

        if(SegFaultErr == RAM_SEG_FAULT)
            printf(colorize("RAM SEGMENTATION FAULT, aborted.\n", _RED_));
        
        else if(SegFaultErr == VRAM_SEG_FAULT)
            printf(colorize("VRAM SEGMENTATION FAULT, aborted.\n", _RED_));

        return;
    }

    Err->stack = StackPush(&(SPU->Stk), Mem[SPU->Regs[SPU->Code[++SPU->InstrPtr]]]);

    SPU->InstrPtr++;
}

static void DoPopm(processor *SPU, long long int *Mem, const unsigned int MemSize, 
                    const processor_err_t SegFaultErr, processor_err_struct_t *Err)
{
    assert(SPU);
    assert(Err);
    assert(Mem);

    if(SPU->Code[SPU->InstrPtr + 1] >= (long long int)NUM_OF_REGS || SPU->Code[SPU->InstrPtr + 1] < 0)
    {
        Err->proc = WRONG_REGS;

        printf(colorize("Wrong bytecode, aborted\n", _RED_));

        return;
    }

    if(SPU->Regs[SPU->Code[SPU->InstrPtr + 1]] >= MemSize || SPU->Regs[SPU->Code[SPU->InstrPtr + 1]] < 0)
    {
        Err->proc = SegFaultErr;

        if(SegFaultErr == RAM_SEG_FAULT)
            printf(colorize("RAM SEGMENTATION FAULT, aborted.\n", _RED_));
        
        else if(SegFaultErr == VRAM_SEG_FAULT)
            printf(colorize("VRAM SEGMENTATION FAULT, aborted.\n", _RED_));

        return;
    }

    Err->stack = StackPop(&(SPU->Stk), Mem + SPU->Regs[SPU->Code[++SPU->InstrPtr]]);

    SPU->InstrPtr++;
}

static void DoDrw(processor *SPU, processor_err_struct_t *Err, WINDOW **Win)
{
    assert(SPU);
    assert(Err);
    assert(Win);

    if(*Win == NULL)
    {
        *Win = GraphicsInit();
        if(*Win == NULL)
        {
            //printf("Hello1\n");
            Err->proc = CONSOLE_ERR;

            return;
        }
    }

    if(Draw(SPU->VRAM, *Win))
    {
        //printf("Hello2\n");
        Err->proc = CONSOLE_ERR;

        return;
    }

    SPU->InstrPtr++;
}

