#include "StrFunctions.h"

char *SkipSpaces(char *Cursor)
{
    assert(Cursor);

    while(*Cursor == ' ' || *Cursor == '\t')
        Cursor++;

    return Cursor;
}

char *CompareAndMove(char *CompInstr, const char *RefInstr)
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

    CompInstr = SkipSpaces(CompInstr);

    if(*CompInstr == '\n' || *CompInstr == '\0')
    {
        *Instr = _SKIP_LINE;

        
        return NULL;
    }

    for (size_t i = 0; i < COMMANDS_SIZE; i++)
    {
        if((PtrToEnd = CompareAndMove(CompInstr, COMMANDS[i])))
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

    //printf("%s\n", CompReg);

    for (size_t i = 0; i < NUM_OF_REGS; i++)
    {
        if((PtrToEnd = CompareAndMove(CompReg, REGS_NAME[i])))
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


bool CheckEndLine(char *Line)
{
    if(Line == NULL)
        return true;

    while (*Line != '\n' && *Line != '\0' && *Line != EOF)
    {
        if(*Line != ' ' && *Line != '\t')
            return true;

        Line++;
    }

    return false;
}

unsigned int CountInstrInLine(char *Cursor)
{
    assert(Cursor);

    instruction Instr = _UNDEF;

    RecognizeAndMove(Cursor, &Instr);

    switch (Instr)
    {
    case OUT:
    case HLT:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case SQRT:
    case RET:
    case DRW:
    case PAUSE:

        return 1;
        break;
    
    case PUSH:
    case PUSHR:
    case POPR:
    case PUSHM:
    case POPM:
    case PUSHV:
    case POPV:
    case JMP:
    case JB:
    case JBE:
    case JA:
    case JAE:
    case JE:
    case JNE:
    case CALL:

        return 2;
        break;
    
    case _SKIP_LINE:
    case _UNDEF:
    default:

        return 0;
        break;
    }

    return (unsigned int)-1;
}

char *RecognizeLabelAndMove(char *Cursor, label *Label, size_t NumOfLabels, long long int *LabelLoc)
{
    assert(Cursor);
    assert(Label);
    assert(LabelLoc);

    char *Cursor1 = NULL;

    for (size_t i = 0; i < NumOfLabels; i++)
    {
        if((Cursor1 = CompareAndMove(Cursor, Label[i].Name)))
        {
            *LabelLoc = Label[i].Loc;

            return Cursor1;
        }
    }

    return NULL;
}

bool MakeLabel(char *Cursor, label *Label, size_t *NumOfLabels,
                long long int InstrCount, unsigned int LineCount, int *SameLabelIndx)
{
    assert(Cursor);
    assert(Label);
    assert(NumOfLabels);
    assert(SameLabelIndx);

    *SameLabelIndx = -1;

    size_t i = 0;
    while(i < MAX_LABEL_LENGTH && Cursor[i] > ' ')
    {
        Label[*NumOfLabels].Name[i] = Cursor[i];

        i++;
    }

    if(i == 0 || i == MAX_LABEL_LENGTH)
        return true;

    Label[*NumOfLabels].Name[i] = '\0';
    Label[*NumOfLabels].Loc = InstrCount;
    Label[*NumOfLabels].LocInLine = LineCount;

    for (int li = 0; li < (long long int)*NumOfLabels; li++)
    {
        if(strcmp(Label[*NumOfLabels].Name, Label[li].Name) == 0)
        {
            *SameLabelIndx = li;

            return true;
        }
    }

    (*NumOfLabels)++;

    return CheckEndLine(Cursor + i);

}

