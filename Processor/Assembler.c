#include "Assembler.h"

#include "AssemblerDo.c"

static bool PreAssemble(const char *InFilePath, TableOfContent *CodeLine[], unsigned int *NumOfLines, int *LabelsArr[])
{
    if(InFilePath == NULL)
        return true;

    if(CodeLine == NULL)
        return true;
    
    if(LabelsArr == NULL)
        return true;

    //-------------------------------------OPEN--------------------------------
    char *Cursor = CreateStrBuffer(InFilePath);

    if(Cursor == NULL)
    {
        _print_err("File(s) can't open\n");

        return true;
    }

    (*CodeLine) = CreateTOC(Cursor, NumOfLines, '\n');
    
    if(CodeLine == NULL)
    {
        _print_err("File is empty\n");

        return true;
    }

    struct stat FileInfo = {};
    if(stat(InFilePath, &FileInfo))
    {
        _print_err("File(s) can't open\n");

        free(Cursor);

        return true;
    }

    *LabelsArr = (int *)calloc(LABLES_SIZE, sizeof(int));

    if(*LabelsArr == NULL)
        return true;
    //-------------------------------------------------------------------------

    ExcludeComments(Cursor);

    unsigned int InstrCounter = 0;
    long long int Lable = 0;

    for (unsigned int i = 0; i < *NumOfLines; i++)
    {
        Cursor = (*CodeLine)[i].Line;

        Cursor = SkipSpaces(Cursor);

        if(*Cursor == ':')
        {
            *Cursor++ = '\0';
            Cursor = AtoIAndMove(Cursor, &Lable);
            Cursor = SkipSpaces(Cursor);

            if(CheckEndLine(Cursor) || Lable >= LABLES_SIZE || Lable < 0)
            {
                _print_err("?lable?\t");
                _print_location(InFilePath, i);

                free(Cursor);

                return true;
            }

            (*LabelsArr)[Lable] = (int)InstrCounter;
        }

        else
            InstrCounter += CountInstrInLine(Cursor);
    }

    return false;
}



bool Assemble(const char *InFilePath, const char *OutFilePath)
{
    //------------------------open files------------------------
    assert(InFilePath);
    assert(OutFilePath);

    bool ErrorStatus = false;

    TableOfContent *CodeLine = NULL;

    unsigned int NumOfLines = 0;

    int *LabelsArr = NULL;

    if(PreAssemble(InFilePath, &CodeLine, &NumOfLines, &LabelsArr))
    {
        free(CodeLine[0].Line);
        free(CodeLine);
        free(LabelsArr);

        return true;
    }

    assert(LabelsArr);

    FILE *Out = fopen(OutFilePath, "w");  
    if(Out == NULL)
    {
        _print_err("File(s) can't be opened\n");

        free(CodeLine[0].Line);
        free(CodeLine);
        free(LabelsArr);

        return true;
    }

    //--------------------start assembly------------------------

    printf(colorize("\n\nASSEMBLING IS IN PROGRESS.......", _CYAN_));

    char *Cursor = NULL;
    instruction Instr = _UNDEF;
    
    for (unsigned int i = 0; i < NumOfLines; i++)
    {
        Cursor = RecognizeAndMove(CodeLine[i].Line, &Instr);

        switch(Instr)
        {
        case PUSHR:

            if(DoPushrPoprInstr(Cursor, Out, PUSHR))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: pushr ?register?\t");
                _print_location(InFilePath, i);
            }

            break;

        case POPR:

            if(DoPushrPoprInstr(Cursor, Out, POPR))
            {
                ErrorStatus = true;

                //printf("%s\n", Cursor);

                _print_err("Invalid syntax: popr ?register?\t");
                _print_location(InFilePath, i);
            }

            break;

        case PUSH:

            if(DoPush(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: push ?pushing value?\t");
                _print_location(InFilePath, i);
            }

            break;
        
        case PUSHM:

            if(DoPushmPopmInstr(Cursor, Out, PUSHM))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: pushm [?register?]\t");
                _print_location(InFilePath, i);
            }

            break;
        
        case POPM:

            if(DoPushmPopmInstr(Cursor, Out, POPM))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: popm [?register?]\t");
                _print_location(InFilePath, i);
            }

            break;

        case JMP:
        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
        case CALL:

            if (DoJmpInstr(Cursor, Out, Instr, LabelsArr))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: <goto> ?:label?\t");
                _print_location(InFilePath, i);
            }

            break;

        case _UNDEF:

            
            _print_err("?instruction?\t");
            _print_location(InFilePath, i);

            ErrorStatus = true;

            break;
        
        case _SKIP_LINE:
            break;
        
        case OUT:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case SQRT:
        case HLT:
        case RET:
        default:

            if(DoInstrWithoutArg(Cursor, Out, Instr))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax\t");
                _print_location(InFilePath, i);

            }

            break;
        }
    }

    if(CodeLine)
        free(CodeLine[0].Line);   
    free(CodeLine);
    free(LabelsArr);
    fclose(Out);

    if(ErrorStatus)
    {
        DisAssemble(OutFilePath);
        printf(colorize("Here's your govnocode. Are you silly?\n", _GRAY_));

        Out = fopen(OutFilePath, "w");
        fclose(Out);
        printf(colorize("TRANSLATION ERRORS\n", _RED_));
    }

    else
        printf(colorize("ASSEMBLING FINISHED\n", _GREEN_));

    return ErrorStatus;
}

