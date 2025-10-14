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

    if(Err.stack == NO_ERR)
        Err.stack = StackVerify(&(Proc->RetAddr));

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

    printf(colorize("ProcessorDump called from %s:%u\n", _GRAY_), File, Line);

    if (Err->proc || Err->stack)
        printf("Stack Error: %d\nProcessor Error: %d\n", Err->stack, Err->proc);

    printf( colorize("processor [%#lx]\n{\n", _CYAN_)
            colorize("NumOfInstr=%lu\nInstrPtr=%lu\n", _MAGENTA_)
            colorize("Regs[%#lx]\n{\n", _YELLOW_),
            (size_t)Proc,
            Proc->_NumOfInstr, Proc->InstrPtr,
            (size_t)Proc->Regs);

    for (size_t i = 0; i < NUM_OF_REGS; i++)
        printf(colorize("%s ", _MAGENTA_) colorize("%lld\n", _CYAN_), REGS_NAME[i], Proc->Regs[i]);

    printf(colorize("}\n", _YELLOW_) "Do you want to see the Stk? [Y/N]\n");

    char c = 0;
    scanf("%c", &c);
    if (c == 'Y')
    {
        StackDump(&(Proc->Stk));
    }


    printf(colorize("}\n", _YELLOW_) "Do you want to see the RetAddr stack? [Y/N]\n");
    getchar();

    c = 0;
    scanf("%c", &c);
    if (c == 'Y')
    {
        StackDump(&(Proc->RetAddr));
    }


    printf(colorize("Code[%#lx]\n{\n", _YELLOW_) "Do you want to see the Code? [Y/N]\n", (size_t)Proc->Code);
    getchar();

    c = 0;
    scanf("%c", &c);
    if (c == 'Y')
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

        fclose(BinFile);

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

    fclose(BinFile);

    if(ProcErr)
    {
        ProcErr->stack = StackInit(&(Proc->Stk), Proc->_NumOfInstr + 1);

        if(ProcErr->stack)
            return true;

        ProcErr->stack = StackInit(&(Proc->RetAddr), Proc->_NumOfInstr);

        if(ProcErr->stack)
            return true;
    }

    else
    {
        if(StackInit(&(Proc->Stk), Proc->_NumOfInstr + 1))
            return true;
        
        if(StackInit(&(Proc->RetAddr), Proc->_NumOfInstr))
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

    StackDestroy(&(Proc->RetAddr));

    free(Proc->Code);

    Proc->Code = NULL;
}


bool LaunchProgram(processor *Proc, processor_err_struct_t *Err)
{
    if(Err == NULL)
        return true;

    if(ProcessorVerifyExt(Proc, Err))
        return true;

    Proc->InstrPtr = 0;
    while(Proc->InstrPtr < Proc->_NumOfInstr)
    {
        switch(Proc->Code[Proc->InstrPtr])
        {
        case PUSH:

            DoPush(Proc, Err);

            break;
      
        case OUT:

            DoOut(Proc, Err);

            break;
        
        case HLT:

            if(Err->proc == PROC_NO_ERR && Err->stack == NO_ERR)
            {
                #ifdef DEBUG_MODE
                ProcessorDump(Proc, Err);
                #endif

                printf(colorize("Program finished\n", _GRAY_));
                
                return false;
            }

            #ifdef DEBUG_MODE
            ProcessorDump(Proc, Err);
            #endif

            return true;

            break;
        
        case ADD:

            DoAdd(Proc, Err);

            break;
        
        case SUB:

            DoSub(Proc, Err);

            break;
        
        case MUL:

            DoMul(Proc, Err);

            break;
        
        case DIV:

            DoDiv(Proc, Err);

            break;
        
        case SQRT:

            DoSqrt(Proc, Err);

            break;
        
        case PUSHR:

            DoPushr(Proc, Err);

            break;
        
        case POPR:

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
        
        case CALL:

            DoCall(Proc, Err);

            break;

        case RET:

            DoRet(Proc, Err);

            break;

        default:

            printf(colorize("Wrong bytecode, aborted\n", _RED_));

            return true;

            break;
        }

        #ifdef DEBUG_MODE
        ProcessorDump(Proc, Err);
        
        printf("Continue?\n");

        getchar();
        getchar();
        #endif

        if(Err->proc || Err->stack)
            return true;
        
        if(ProcessorVerifyExt(Proc, Err))
            return true;
    }

    printf(colorize("Program finished\n", _GRAY_));

    return false;
}
