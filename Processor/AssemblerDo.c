#pragma once

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

