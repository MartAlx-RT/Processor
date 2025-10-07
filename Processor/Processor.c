#include "Processor.h"

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

    if(Proc->Regs == NULL || sizeof(Proc->Regs) / sizeof(long long int) != NUM_OF_REGS)
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

bool _ProcessorDump(processor *Proc, const char *File, const int Line)
{
    if(Proc == NULL)
        return true;

    //printf(colorize("PROCESSOR:\n", _YELLOW_));

    printf( colorize("ProcessorDump called from %s:%u\n", _GRAY_)
            colorize("processor [%#lx]\n{\n", _CYAN_)
            colorize("NumOfInstr=%lu\nInstrPtr=%lu\n", _MAGENTA_)
            colorize("Regs[%#lx]\n{\n", _YELLOW_),
            File, Line,
            (size_t)Proc,
            Proc->_NumOfInstr, Proc->InstrPtr,
            (size_t)Proc->Regs);

    if(Proc->Regs == NULL)
        return true;

    for (int i = 0; i < NUM_OF_REGS; i++)
        printf(colorize("%s ", _MAGENTA_) colorize("%lld\n", _CYAN_), REGS_NAME[i], Proc->Regs[i]);

    //printf(colorize("STACK:\n", _YELLOW_));
    printf(colorize("}\n", _YELLOW_) "Do you want to see the Stk? [Y/N]\n");

    if(getchar() == 'Y')
    {
        StackDump(&(Proc->Stk));
    }
    //printf(colorize("CODE:\n", _YELLOW_));
    printf(colorize("Code[%#lx]\n{\n", _YELLOW_) "Do you want to see the Code? [Y/N]\n", (size_t)Proc->Code);

    getchar();//whyyyyyyyyyyyyy
    
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

    Proc->Code = (long long int *)calloc(FileInfo.st_size * 2, sizeof(long long int));

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

bool LaunchProgram(processor *Proc, processor_err_struct_t *Err)
{
    assert(Err);

    if(ProcessorVerifyExt(Proc, Err))
        return true;

    Proc->InstrPtr = 0;

    long long int Val = 0, Val1 = 0, Val2 = 0;

    while(Proc->InstrPtr < Proc->_NumOfInstr)
    {
        switch(Proc->Code[Proc->InstrPtr])
        {
        case PUSH:

            Err->stack = StackPush(&(Proc->Stk), Proc->Code[++Proc->InstrPtr]);

            Proc->InstrPtr++;

            break;
        
        case OUT:

            Err->stack = StackPop(&(Proc->Stk), &Val);

            printf("%lld\n", Val);

            Proc->InstrPtr++;

            break;
        
        case HLT:

            if(Err->proc == PROC_NO_ERR && Err->stack == NO_ERR)
                return false;

            return true;

            break;
        
        case ADD:

            Err->stack = StackPop(&(Proc->Stk), &Val1);

            if(Err->stack)
                return true;
            
            Err->stack = StackPop(&(Proc->Stk), &Val2);

            if(Err->stack)
                return true;

            Err->stack = StackPush(&(Proc->Stk), Val1 + Val2);

            Proc->InstrPtr++;

            break;
        
        case SUB:

            Err->stack = StackPop(&(Proc->Stk), &Val1);

            if(Err->stack)
                return true;
            
            Err->stack = StackPop(&(Proc->Stk), &Val2);

            if(Err->stack)
                return true;

            Err->stack = StackPush(&(Proc->Stk), Val2 - Val1);

            Proc->InstrPtr++;

            break;
        
        case MUL:

            Err->stack = StackPop(&(Proc->Stk), &Val1);

            if(Err->stack)
                return true;
            
            Err->stack = StackPop(&(Proc->Stk), &Val2);

            if(Err->stack)
                return true;

            Err->stack = StackPush(&(Proc->Stk), Val1 * Val2);

            Proc->InstrPtr++;

            break;
        
        case DIV:

            Err->stack = StackPop(&(Proc->Stk), &Val1);

            if(Err->stack)
                return true;
            
            Err->stack = StackPop(&(Proc->Stk), &Val2);

            if(Err->stack)
                return true;

            if(Val2 == 0)
            {
                printf(colorize("Divizion by zero, aborted\n", _RED_));

                return true;
            }

            Err->stack = StackPush(&(Proc->Stk), Val2 / Val1);

            Proc->InstrPtr++;

            break;
        
        case SQRT:

            Err->stack = StackPop(&(Proc->Stk), &Val);

            if(Err->stack)
                return true;

            if(Val < 0)
            {
                printf(colorize("SQRT of negative number, aborted\n", _RED_));

                return true;
            }

            Err->stack = StackPush(&(Proc->Stk), (long long int)sqrt((double)Val));

            Proc->InstrPtr++;

            break;
        
        case PUSHR:

            Err->stack = StackPush(&(Proc->Stk), Proc->Regs[Proc->Code[++Proc->InstrPtr]]);

            Proc->InstrPtr++;

            break;
        
        case POPR:

            Err->stack = StackPop(&(Proc->Stk), &(Proc->Regs[Proc->Code[++Proc->InstrPtr]]));

            Proc->InstrPtr++;

            break;
        
        default:

            printf(colorize("Wrong bytecode, aborted\n", _RED_));

            return true;

            break;
        }

        if(Err->proc || Err->stack)
            return true;

        // ProcessorDump(Proc);

        // printf("Continue? ");

        // getchar();
    }

    printf(colorize("Program finished\n", _GRAY_));

    return false;
}
