#include "Processor.h"

#include "ProcessorDo.c"

bool LaunchProgram(processor *SPU, processor_err_struct_t *Err)
{
    if(Err == NULL)
        return true;

    if(ProcessorVerifyExt(SPU, Err))
        return true;

    
    WINDOW *Win = NULL;

    SPU->InstrPtr = 0;
    while(SPU->InstrPtr < SPU->_NumOfInstr)
    {
        switch(SPU->Code[SPU->InstrPtr])
        {
        case PUSH:

            DoPush(SPU, Err);

            break;
      
        case OUT:

            DoOut(SPU, Err);

            break;
        
        case HLT:

            if(Err->proc == PROC_NO_ERR && Err->stack == NO_ERR)
            {
                #ifdef DEBUG_MODE
                ProcessorDump(SPU, Err);
                #endif

                GraphicsDestroy(&Win);
                
                printf(colorize("Program finished\n", _GRAY_));

                return false;
            }

            GraphicsDestroy(&Win);

#ifdef DEBUG_MODE
            ProcessorDump(SPU, Err);
#endif

            return true;

            break;
        
        case ADD:

            DoAdd(SPU, Err);

            break;
        
        case SUB:

            DoSub(SPU, Err);

            break;
        
        case MUL:

            DoMul(SPU, Err);

            break;
        
        case DIV:

            DoDiv(SPU, Err);

            break;
        
        case SQRT:

            DoSqrt(SPU, Err);

            break;
        
        case PUSHR:

            DoPushr(SPU, Err);

            break;
        
        case POPR:

            DoPopr(SPU, Err);

            break;
        
        case JMP:
        
            DoJmp(SPU, Err, NULL);

            break;

        case JB:

            DoJmp(SPU, Err, Below);

            break;

        case JBE:

            DoJmp(SPU, Err, BelowOrEqual);

            break;
        
        case JA:

            DoJmp(SPU, Err, Above);

            break;
        
        case JAE:

            DoJmp(SPU, Err, AboveOrEqual);

            break;
        
        case JE:

            DoJmp(SPU, Err, Equal);

            break;
        
        case JNE:

            DoJmp(SPU, Err, NotEqual);

            break;
        
        case CALL:

            DoCall(SPU, Err);

            break;

        case RET:

            DoRet(SPU, Err);

            break;
        
        case PUSHM:

            DoPushm(SPU, SPU->RAM, RAM_SIZE, RAM_SEG_FAULT, Err);

            break;
        
        case POPM:

            DoPopm(SPU, SPU->RAM, RAM_SIZE, RAM_SEG_FAULT, Err);

            break;
        
        case PUSHV:

            DoPushm(SPU, SPU->VRAM, VRAM_SIZE, VRAM_SEG_FAULT, Err);

            break;
        
        case POPV:

            DoPopm(SPU, SPU->VRAM, VRAM_SIZE, VRAM_SEG_FAULT, Err);

            break;
        
        case DRW:

            DoDrw(SPU, Err, &Win);

            break;
        
        case PAUSE:

            assert(Win);
            if (Win)
                getch();
            
            else
                getchar();

            SPU->InstrPtr++;

            // printw("press any key for quit\n");
            // refresh();

            break;

        default:

            GraphicsDestroy(&Win);

            printf(colorize("Wrong bytecode, aborted\n", _RED_));

            return true;

            break;
        }

#ifdef DEBUG_MODE
        ProcessorDump(SPU, Err);
        
        printf("Continue?\n");

        getchar();
        getchar();
#endif

        if(Err->proc || Err->stack)
        {
            GraphicsDestroy(&Win);
            
            return true;
        }
        
        if(ProcessorVerifyExt(SPU, Err))
            return true;
    }

    GraphicsDestroy(&Win);

    printf(colorize("Missing 'hlt', program finished\n", _GRAY_));

    return false;
}
