#include "Processor.h"

#include "ProcessorDo.c"
#include "JmpComparators.c"

bool ProcessorVerifyExt(processor *Proc, processor_err_struct_t *ProcErr)
{

    

    if (Proc == NULL)
    {
        if(ProcErr)
        {
            ProcErr->proc = NULLPTR;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }

    bool ErrStatus = false;
    processor_err_struct_t Err = {};

    Err.stack = StackVerify(&(Proc->Stk));

    if(Err.stack)
        ErrStatus = true;

    if(Proc->_NumOfInstr <= Proc->InstrPtr)
    {
        Err.proc = WRONG_IP;
    
        ErrStatus = true;
    }

    if(Proc->Code == NULL)
    {
        Err.proc = CODE_NULLPTR;

        ErrStatus = true;
    }

    if(sizeof(Proc->Regs) / sizeof(long long int) != NUM_OF_REGS)
    {
        Err.proc = WRONG_REGS;

        ErrStatus = true;
    }

    if (ProcErr)
            *ProcErr = Err;

    return ErrStatus;
}

bool ProcessorVerify(processor *Proc)
{
    return ProcessorVerifyExt(Proc, NULL);
}

bool _ProcessorDump(processor *Proc, processor_err_struct_t *Err, const char *File, const unsigned int Line)
{
    if(Proc == NULL)
        return true;

    //printf(colorize("PROCESSOR:\n", _YELLOW_));
    printf(colorize("ProcessorDump called from %s:%u\n", _GRAY_), File, Line);

    if (Err->proc || Err->stack)
        printf("Stack Error: %d\nProcessor Error: %d\n", Err->stack, Err->proc);
    

    printf( colorize("processor [%#lx]\n{\n", _CYAN_)
            colorize("NumOfInstr=%lu\nInstrPtr=%lu\n", _MAGENTA_)
            colorize("Regs[%#lx]\n{\n", _YELLOW_),
            (size_t)Proc,
            Proc->_NumOfInstr, Proc->InstrPtr,
            (size_t)Proc->Regs);

    // if(Proc->Regs == NULL)
    //     return true;

    for (size_t i = 0; i < NUM_OF_REGS; i++)
        printf(colorize("%s ", _MAGENTA_) colorize("%lld\n", _CYAN_), REGS_NAME[i], Proc->Regs[i]);

    //printf(colorize("STACK:\n", _YELLOW_));
    printf(colorize("}\n", _YELLOW_) "Do you want to see the Stk? [Y/N]\n");

    if(getchar() == 'Y')
    {
        StackDump(&(Proc->Stk));
    }
    //printf(colorize("CODE:\n", _YELLOW_));
    printf(colorize("Code[%#lx]\n{\n", _YELLOW_) "Do you want to see the Code? [Y/N]\n", (size_t)Proc->Code);

    getchar();//scanf()!!
    
    if (getchar() == 'Y')
    {
        for (size_t i = 0; i < Proc->_NumOfInstr; i++)
            printf(colorize("[%lu]=%lld\n", _GREEN_), i, Proc->Code[i]);
    }

    printf(colorize("}\n", _YELLOW_) colorize("}\n", _CYAN_) colorize("ProcessorDump has finished\n", _GRAY_));

    return false;
}

bool ProcessorInitExt(processor *Proc, const char *BinFilePath, processor_err_struct_t *ProcErr)
{
    if (Proc == NULL)
    {
        if(ProcErr)
        {
            ProcErr->proc = NULLPTR;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }
    
    if(BinFilePath == NULL)
    {
        if(ProcErr)
        {
            ProcErr->proc = WRONG_FILE;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }

    FILE *BinFile = fopen(BinFilePath, "r");

    if(BinFile == NULL)
    {
        if(ProcErr)
        {
            ProcErr->proc = WRONG_FILE;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }

    struct stat FileInfo = {};

    if(stat(BinFilePath, &FileInfo))
    {
        if(ProcErr)
        {
            ProcErr->proc = WRONG_FILE;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }

    Proc->Code = (long long int *)calloc((unsigned long int)FileInfo.st_size, sizeof(long long int));

    if(Proc->Code == NULL)
    {
        if(ProcErr)
        {
            ProcErr->proc = CODE_NULLPTR;
            ProcErr->stack = NO_ERR;
        }

        return true;
    }

    long long int Num = 0;

    Proc->_NumOfInstr = 0;

    while(fscanf(BinFile, "%lld", &Num) > 0)
        Proc->Code[Proc->_NumOfInstr++] = Num;

    Proc->InstrPtr = 0;

    if(ProcErr)
    {
        ProcErr->stack = StackInit(&(Proc->Stk), Proc->_NumOfInstr + 1);

        if(ProcErr->stack)
            return true;
    }

    else
    {
        if(StackInit(&(Proc->Stk), Proc->_NumOfInstr + 1))
            return true;
    }

    if(ProcErr)
    {
        return ProcessorVerifyExt(Proc, ProcErr);
    }

    return ProcessorVerify(Proc);
}

bool ProcessorInit(processor *Proc, const char *BinFilePath)
{
    return ProcessorInitExt(Proc, BinFilePath, NULL);
}

void ProcessorDestroy(processor *Proc)
{
    if(Proc == NULL)
        return;

    Proc->_NumOfInstr = Proc->InstrPtr = 0;

    StackDestroy(&(Proc->Stk));

    free(Proc->Code);

    Proc->Code = NULL;
}

/*
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


static bool Below(long long int Val1, long long int Val2)
{
    return (Val1 < Val2);
}

static bool BelowOrEqual(long long int Val1, long long int Val2)
{
    return (Val1 <= Val2);
}

static bool Above(long long int Val1, long long int Val2)
{
    return (Val1 > Val2);
}

static bool AboveOrEqual(long long int Val1, long long int Val2)
{
    return (Val1 >= Val2);
}

static bool Equal(long long int Val1, long long int Val2)
{
    return (Val1 == Val2);
}

static bool NotEqual(long long int Val1, long long int Val2)
{
    return (Val1 != Val2);
}
*/

bool LaunchProgram(processor *Proc, processor_err_struct_t *Err)
{
    //getchar();
    assert(Err);

    if(ProcessorVerifyExt(Proc, Err))
        return true;

    Proc->InstrPtr = 0;

    //long long int Val = 0, Val1 = 0, Val2 = 0;

    while(Proc->InstrPtr < Proc->_NumOfInstr)
    {
        switch(Proc->Code[Proc->InstrPtr])
        {
        case PUSH:

            // Err->stack = StackPush(&(Proc->Stk), Proc->Code[++Proc->InstrPtr]);

            // Proc->InstrPtr++;

            DoPush(Proc, Err);

            break;
        
        case OUT:

            // Err->stack = StackPop(&(Proc->Stk), &Val);

            // printf("%lld\n", Val);

            // Proc->InstrPtr++;

            DoOut(Proc, Err);

            break;
        
        case HLT:

            if(Err->proc == PROC_NO_ERR && Err->stack == NO_ERR)
                return false;

            return true;

            break;
        
        case ADD:

            // Err->stack = StackPop(&(Proc->Stk), &Val1);

            // if(Err->stack)
            //     return true;
            
            // Err->stack = StackPop(&(Proc->Stk), &Val2);

            // if(Err->stack)
            //     return true;

            // Err->stack = StackPush(&(Proc->Stk), Val1 + Val2);

            // Proc->InstrPtr++;

            DoAdd(Proc, Err);

            break;
        
        case SUB:

            // Err->stack = StackPop(&(Proc->Stk), &Val1);

            // if(Err->stack)
            //     return true;
            
            // Err->stack = StackPop(&(Proc->Stk), &Val2);

            // if(Err->stack)
            //     return true;

            // Err->stack = StackPush(&(Proc->Stk), Val2 - Val1);

            // Proc->InstrPtr++;

            DoSub(Proc, Err);

            break;
        
        case MUL:

            // Err->stack = StackPop(&(Proc->Stk), &Val1);

            // if(Err->stack)
            //     return true;
            
            // Err->stack = StackPop(&(Proc->Stk), &Val2);

            // if(Err->stack)
            //     return true;

            // Err->stack = StackPush(&(Proc->Stk), Val1 * Val2);

            // Proc->InstrPtr++;

            DoMul(Proc, Err);

            break;
        
        case DIV:

            // Err->stack = StackPop(&(Proc->Stk), &Val1);

            // if(Err->stack)
            //     return true;
            
            // Err->stack = StackPop(&(Proc->Stk), &Val2);

            // if(Err->stack)
            //     return true;

            // if(Val2 == 0)
            // {
            //     printf(colorize("Divizion by zero, aborted\n", _RED_));

            //     return true;
            // }

            // Err->stack = StackPush(&(Proc->Stk), Val2 / Val1);

            // Proc->InstrPtr++;

            DoDiv(Proc, Err);

            break;
        
        case SQRT:

            // Err->stack = StackPop(&(Proc->Stk), &Val);

            // if(Err->stack)
            //     return true;

            // if(Val < 0)
            // {
            //     printf(colorize("SQRT of negative number, aborted\n", _RED_));

            //     return true;
            // }

            // Err->stack = StackPush(&(Proc->Stk), (long long int)sqrt((double)Val));

            // Proc->InstrPtr++;

            DoSqrt(Proc, Err);

            break;
        
        case PUSHR:

            // Err->stack = StackPush(&(Proc->Stk), Proc->Regs[Proc->Code[++Proc->InstrPtr]]);

            // Proc->InstrPtr++;

            DoPushr(Proc, Err);

            break;
        
        case POPR:

            // Err->stack = StackPop(&(Proc->Stk), &(Proc->Regs[Proc->Code[++Proc->InstrPtr]]));

            // Proc->InstrPtr++;

            DoPopr(Proc, Err);

            break;
        
        case JMP:

            DoJmp(Proc, Err, NULL);

            break;

        case JB:

            DoJmp(Proc, Err, Below);

            break;

        case JBE:

            DoJmp(Proc, Err, BelowOrEqual);

            break;
        
        case JA:

            DoJmp(Proc, Err, Above);

            break;
        
        case JAE:

            DoJmp(Proc, Err, AboveOrEqual);

            break;
        
        case JE:

            DoJmp(Proc, Err, Equal);

            break;
        
        case JNE:

            DoJmp(Proc, Err, NotEqual);

            break;

        default:

            printf(colorize("Wrong bytecode, aborted\n", _RED_));

            return true;

            break;
        }

        if(Err->proc || Err->stack)
            return true;
        
        if(ProcessorVerifyExt(Proc, Err))
            return true;

        // ProcessorDump(Proc);

        // printf("Continue? ");

        // getchar();
    }

    printf(colorize("Program finished\n", _GRAY_));

    return false;
}
