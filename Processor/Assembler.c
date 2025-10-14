#include "Assembler.h"

#include "AssemblerDo.c"
/*  
static bool DoPush(char *Cursor, FILE *Out)
{

    long long int Arg = 0;

    assert(Cursor);
    assert(Out);

    if (Cursor[0] == ' ')
    {
        Cursor++;
        Cursor = AtoIAndMove(Cursor, &Arg);
        if(Cursor == NULL || CheckEndLine(Cursor))
        {
            
            
            
            //ErrorStatus = true;
            return true;
        }
        fprintf(Out, "%d %lld ", (int)PUSH, Arg);
        return false;
    }

    return true;
    // ErrorStatus = true;
}

static bool DoInstrWithoutArg(char *Cursor, FILE *Out, instruction Instr)
{
    assert(Cursor);
    assert(Out);

    if (CheckEndLine(Cursor))
    {

    // _print_err("Invalid syntax\t");
    // _print_location(InFilePath, i);

        return true;
    }

    fprintf(Out, "%d ", Instr);

    return false;
}

static bool DoPushr(char *Cursor, FILE *Out)
{
    assert(Cursor);
    assert(Out);

    regs Reg = UNDEF_REG;

    if (Cursor[0] == ' ')
    {
        Cursor++;
        Cursor = RecognizeRegAndMove(Cursor, &Reg);

        if(Cursor == NULL || CheckEndLine(Cursor))
        {

            //printf("Error here\n");

            //ErrorStatus = true;
            return true;
        }
        fprintf(Out, "%d %d ", (int)PUSHR, (int)Reg);
        return false;
    }

    return true;

}

static bool DoPopr(char *Cursor, FILE *Out)
{
    assert(Cursor);
    assert(Out);

    regs Reg = UNDEF_REG;

    if (Cursor[0] == ' ')
    {
        Cursor++;
        Cursor = RecognizeRegAndMove(Cursor, &Reg);
        //assert(Cursor);
        if (Cursor == NULL || CheckEndLine(Cursor))
        {

            //printf("Err here\n");

            // ErrorStatus = true;
            return true;
        }
        fprintf(Out, "%d %d ", (int)POPR, (int)Reg);
        return false;
    }

    return true;
}

static bool DoJmpInstr(char *Cursor, FILE *Out, instruction Instr)
{
    assert(Cursor);
    assert(Out);

    long long int Lable = 0;

    if(Cursor[0] == ' ' && Cursor[1] == ':')
    {
        Cursor = AtoIAndMove(Cursor + 2, &Lable);

        if(CheckEndLine(Cursor))
        {
            return true;
        }

        fprintf(Out, "%d %d ", (int)Instr, Lables[Lable]);

        return false;
    }

    return true;
}
*/

bool Assemble(const char *InFilePath, const char *OutFilePath)
{
    //------------------------open files------------------------
    assert(InFilePath);
    assert(OutFilePath);

    bool ErrorStatus = false;

    TableOfContent *CodeLine = NULL;

    unsigned int NumOfLines = 0;

    if(PreAssemble(InFilePath, &CodeLine, &NumOfLines))
    {
        free(CodeLine[0].Line);
        free(CodeLine);

        return true;
    }

    

    FILE *Out = fopen(OutFilePath, "w");

    

    if(Out == NULL)
    {
        _print_err("File(s) can't be opened\n");

        free(CodeLine[0].Line);
        free(CodeLine);

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

            if(DoPushr(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: ?pushing value?\t");
                _print_location(InFilePath, i);
            }

            break;

        case POPR:

            if(DoPopr(Cursor, Out))
            {
                ErrorStatus = true;

                printf("%s\n", Cursor);

                _print_err("Invalid syntax: ?poping value?\t");
                _print_location(InFilePath, i);
            }

            break;

        case PUSH:

            if(DoPush(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: ?pushing value?\t");
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

            //printf("Hello\n");
            if (DoJmpInstr(Cursor, Out, Instr))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: ?lable?\t");
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


    fclose(Out);

    if(CodeLine)
        free(CodeLine[0].Line);
    
    free(CodeLine);

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

void LablesPrint(void)
{
    for (int i = 0; i < 50; i++)
        printf(colorize("[%d]=%d\n", _GREEN_), i, Lables[i]);

    printf(colorize("...\n", _GREEN_));
}

