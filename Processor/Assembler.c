#include "Assembler.h"
  
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
        if(Cursor == NULL || CheckEndLine(Cursor))
        {
            
            
            
            //ErrorStatus = true;
            return true;
        }
        fprintf(Out, "%d %d ", (int)POPR, (int)Reg);
        return false;
    }

    return true;
}

bool Assemble(const char *InFilePath, const char *OutFilePath)
{
    bool ErrorStatus = false;

    assert(InFilePath);
    assert(OutFilePath);

    //-------------------------------------OPEN--------------------------------


    char *Program = CreateStrBuffer(InFilePath);

    //printf("buffersize=%llu\n", strlen(Program));

    FILE *Out = fopen(OutFilePath, "w");

    if(Program == NULL || Out == NULL)
    {
        free(Program);
        fclose(Out);


        _print_err("File(s) can't open\n");

        ErrorStatus = true;

        return ErrorStatus;
    }

    unsigned int NumOfInstructions = 0;

    TableOfContent *Instruction = CreateTOC(Program, &NumOfInstructions, '\n');
    assert(Instruction);    

    ExcludeComments(Program);

    //-------------------------------------------------------------------------

    printf(colorize("\n\nASSEMBLING IS IN PROGRESS.......", _CYAN_));

    

    char *Cursor = NULL;

    instruction Instr = _UNDEF;

    long long int Arg = 0;
    
    for (unsigned int i = 0; i < NumOfInstructions; i++)
    {
        
        Cursor = RecognizeAndMove(Instruction[i].Line, &Instr);

        switch(Instr)
        {
        case PUSHR:

            if(DoPushr(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: pushing value=?\t");
                _print_location(InFilePath, i);
            }

            break;

        case POPR:

            if(DoPopr(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: poping value=?\t");
                _print_location(InFilePath, i);
            }

            break;

        case PUSH:

            if(DoPush(Cursor, Out))
            {
                ErrorStatus = true;

                _print_err("Invalid syntax: pushing value=?\t");
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



    free(Program);
    fclose(Out);
    free(Instruction);

    if(ErrorStatus)
    {
        FILE *OUT = fopen(OutFilePath, "w");
        fputc(EOF, OUT);
        fclose(OUT);

        printf(colorize("TRANSLATION ERRORS\n", _RED_));
    }

    else
        printf(colorize("ASSEMBLING FINISHED\n", _GREEN_));

    return ErrorStatus;
}