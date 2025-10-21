#include "Processor.h"


bool ProcessorVerifyExt(processor *SPU, processor_err_struct_t *ErrPtr)
{
    if (SPU == NULL)
    {
        if(ErrPtr)
        {
            ErrPtr->proc = NULLPTR;
            ErrPtr->stack = NO_ERR;
        }

        return true;
    }

    bool ErrStatus = false;
    processor_err_struct_t Err = {};

    Err.stack = StackVerify(&(SPU->Stk));

    if(Err.stack == NO_ERR)
        Err.stack = StackVerify(&(SPU->RetAddr));

    if(Err.stack)
        ErrStatus = true;

    if(SPU->_NumOfInstr <= SPU->InstrPtr)
    {
        Err.proc = WRONG_IP;
    
        ErrStatus = true;
    }

    if(SPU->Code == NULL)
    {
        Err.proc = CODE_NULLPTR;

        ErrStatus = true;
    }

    if(SPU->RAM == NULL)
    {
        Err.proc = RAM_NULLPTR;

        ErrStatus = true;
    }

    if(SPU->VRAM == NULL)
    {
        Err.proc = VRAM_NULLPTR;

        ErrStatus = true;
    }

    if(sizeof(SPU->Regs) / sizeof(long long int) != NUM_OF_REGS)
    {
        Err.proc = WRONG_REGS;

        ErrStatus = true;
    }

    if (ErrPtr)
            *ErrPtr = Err;

    return ErrStatus;
}

bool ProcessorVerify(processor *SPU)
{
    return ProcessorVerifyExt(SPU, NULL);
}

bool _ProcessorDump(processor *SPU, processor_err_struct_t *Err, const char *File, const unsigned int Line)
{
    if(SPU == NULL)
        return true;

    printf(colorize("ProcessorDump called from %s:%u\n", _GRAY_), File, Line);

    if (Err->proc || Err->stack)
        printf("Stack Error: %d\nProcessor Error: %d\n", Err->stack, Err->proc);

    printf( colorize("processor [%#lx]\n{\n", _CYAN_)
            colorize("NumOfInstr=%lu\nInstrPtr=%lu\n", _MAGENTA_)
            colorize("Regs[%#lx]\n{\n", _YELLOW_),
            (size_t)SPU,
            SPU->_NumOfInstr, SPU->InstrPtr,
            (size_t)SPU->Regs);

    for (size_t i = 0; i < NUM_OF_REGS; i++)
        printf(colorize("%s ", _MAGENTA_) colorize("%lld\n", _CYAN_), REGS_NAME[i], SPU->Regs[i]);

    printf(colorize("}\n", _YELLOW_) "Do you want to see the Stk? [Y/N]\n");

    char c = 0;
    scanf("%c", &c);
    if (c == 'Y')
    {
        StackDump(&(SPU->Stk));
    }


    printf(colorize("}\n", _YELLOW_) "Do you want to see the RetAddr stack? [Y/N]\n");
    getchar();

    c = 0;
    scanf("%c", &c);
    if (c == 'Y')
    {
        StackDump(&(SPU->RetAddr));
    }


    printf(colorize("Code[%#lx]\n{\n", _YELLOW_) "Do you want to see the Code? [Y/N]\n", (size_t)SPU->Code);
    getchar();

    c = 0;
    scanf("%c", &c);
    if (c == 'Y')
    {
        for (size_t i = 0; i < SPU->_NumOfInstr; i++)
            printf(colorize("[%lu]=%lld\n", _GREEN_), i, SPU->Code[i]);
    }

    printf(colorize("}\n", _YELLOW_) colorize("}\n", _CYAN_) colorize("ProcessorDump has finished\n", _GRAY_));

    return false;
}

bool ProcessorInitExt(processor *SPU, const char *BinFilePath, processor_err_struct_t *Err)
{
    if (SPU == NULL)
    {
        if(Err)
        {
            Err->proc = NULLPTR;
            Err->stack = NO_ERR;
        }

        return true;
    }
    if(BinFilePath == NULL)
    {
        if(Err)
        {
            Err->proc = WRONG_FILE;
            Err->stack = NO_ERR;
        }

        return true;
    }

    FILE *BinFile = fopen(BinFilePath, "r");
    if(BinFile == NULL)
    {
        if(Err)
        {
            Err->proc = WRONG_FILE;
            Err->stack = NO_ERR;
        }

        return true;
    }

    struct stat FileInfo = {};
    if(stat(BinFilePath, &FileInfo))
    {
        if(Err)
        {
            Err->proc = WRONG_FILE;
            Err->stack = NO_ERR;
        }

        fclose(BinFile);

        return true;
    }

    SPU->Code = (long long int *)calloc((unsigned long int)FileInfo.st_size, sizeof(long long int));
    if(SPU->Code == NULL)
    {
        if(Err)
        {
            Err->proc = CODE_NULLPTR;
            Err->stack = NO_ERR;
        }

        return true;
    }

    long long int Num = 0;

    SPU->_NumOfInstr = 0;

    while(fscanf(BinFile, "%lld", &Num) > 0)
        SPU->Code[SPU->_NumOfInstr++] = Num;

    SPU->InstrPtr = 0;

    fclose(BinFile);


    SPU->RAM = (long long int *)calloc(RAM_SIZE + 1, sizeof(long long int));
    if(SPU->RAM == NULL)
    {
        if(Err)
        {
            Err->proc = RAM_NULLPTR;
        }
        return true;
    }

    SPU->VRAM = (long long int *)calloc(VRAM_SIZE + 1, sizeof(long long int));
    if(SPU->VRAM == NULL)
    {
        if(Err)
        {
            Err->proc = VRAM_NULLPTR;
        }
        return true;
    }

    if(Err)
    {
        Err->stack = StackInit(&(SPU->Stk), SPU->_NumOfInstr + 1);

        if(Err->stack)
            return true;

        Err->stack = StackInit(&(SPU->RetAddr), SPU->_NumOfInstr);

        if(Err->stack)
            return true;
    }

    else
    {
        if(StackInit(&(SPU->Stk), SPU->_NumOfInstr + 1))
            return true;
        
        if(StackInit(&(SPU->RetAddr), SPU->_NumOfInstr))
            return true;
    }

    if(Err)
    {
        return ProcessorVerifyExt(SPU, Err);
    }

    return ProcessorVerify(SPU);
}

bool ProcessorInit(processor *SPU, const char *BinFilePath)
{
    return ProcessorInitExt(SPU, BinFilePath, NULL);
}

void ProcessorDestroy(processor *SPU)
{
    if(SPU == NULL)
        return;

    SPU->_NumOfInstr = SPU->InstrPtr = 0;

    StackDestroy(&(SPU->Stk));

    StackDestroy(&(SPU->RetAddr));

    free(SPU->Code);
    free(SPU->RAM);
    free(SPU->VRAM);

    SPU->Code = SPU->RAM = SPU->VRAM = NULL;
}
