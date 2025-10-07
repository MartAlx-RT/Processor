#include "StrFunctions.h"

static char *CompareAndMove(char *CompInstr, const char *RefInstr)
{
    assert(CompInstr);
    assert(RefInstr);

    while (*CompInstr == *RefInstr && *CompInstr > ' ')
    {
        CompInstr++;
        RefInstr++;
    }

    if(*CompInstr<=' ' && *RefInstr<=' ')
        return CompInstr;

    return NULL;
}

char *AtoIAndMove(char *Instr, long long int *Num)
{
    assert(Instr);
    assert(Num);

    *Num = 0;

    char Sign = 1;

    if(*Instr == '+')
    {
        Sign = 1;
        Instr++;
    }

    if(*Instr == '-')
    {
        Sign = -1;
        Instr++;
    }

    if(*Instr < '0' || *Instr > '9')
        return NULL;

    while(*Instr >= '0' && *Instr <= '9')
        *Num = 10 * *Num + *Instr++ - '0';
    
    *Num *= Sign;

    return Instr;
}

char *RecognizeAndMove(char *CompInstr, instruction *Instr)
{
    assert(CompInstr);
    assert(COMMANDS);
    assert(Instr);

    char *PtrToEnd = NULL;
    *Instr = _UNDEF;

    if(*CompInstr=='\n' || *CompInstr == '\0')
    {
        *Instr = _SKIP_LINE;

        
        return NULL;
    }

    for (int i = 0; i < COMMANDS_SIZE / sizeof(char *); i++)
    {
        if(PtrToEnd = CompareAndMove(CompInstr, COMMANDS[i]))
        {
            *Instr = (instruction)i;

            return PtrToEnd;
        }
    }


    return NULL;
}

char *RecognizeRegAndMove(char *CompReg, regs *Reg)
{
    assert(CompReg);
    assert(Reg);

    char *PtrToEnd = NULL;
    *Reg = UNDEF_REG;

    if(*CompReg=='\n' || *CompReg == '\0')
    {
        //printf("start with n\n");
        return NULL;
    }

    for (int i = 0; i < NUM_OF_REGS; i++)
    {
        if(PtrToEnd = CompareAndMove(CompReg, REGS_NAME[i]))
        {
            *Reg = (regs)i;

            return PtrToEnd;
        }
    }

    //printf("hz\n");
    return NULL;
}

void ExcludeComments(char *Program)
{
    assert(Program);

    while ((Program = strchr(Program, ';')) != NULL)
    {
        *Program = '\0';
        Program++;
    }
}

//true in error case
bool CheckEndLine(char *Line)
{
    assert(Line);


    while (*Line != '\n' && *Line != '\0' && *Line != EOF)
        if(*Line++ > ' ')
            return true;

    return false;
}