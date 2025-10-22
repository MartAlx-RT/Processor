//#pragma once



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
            return true;
        }
        fprintf(Out, "%d %lld ", (int)PUSH, Arg);
        return false;
    }

    return true;
}

static bool DoInstrWithoutArg(char *Cursor, FILE *Out, instruction Instr)
{
    assert(Cursor);
    assert(Out);

    if (CheckEndLine(Cursor))
    {
        return true;
    }

    fprintf(Out, "%d ", Instr);

    return false;
}

static bool DoJmpInstr(char *Cursor, FILE *Out, instruction Instr, label *Label, size_t NumOfLabels)
{
    assert(Cursor);
    assert(Out);
    assert(Label);
    assert(NumOfLabels);

    //long long int Lable = 0;

    long long int LabelLoc = 0;

    if(Cursor[0] == ' ' && Cursor[1] == ':')
    {
        //Cursor = AtoIAndMove(Cursor + 2, &Lable);

        Cursor = RecognizeLabelAndMove(Cursor + 2, Label, NumOfLabels, &LabelLoc);
        
        if(Cursor == NULL)
        {
            _print_err("undeclared label\t");

            return true;
        }

        if(CheckEndLine(Cursor))
        {
            _print_err("Invalid syntax: <goto> ?:label?\t");

            return true;
        }

        fprintf(Out, "%d %lld ", (int)Instr, LabelLoc);

        return false;
    }

    return true;
}

static bool DoPushrPoprInstr(char *Cursor, FILE *Out, const instruction Instr)
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
            return true;
        }
        fprintf(Out, "%d %d ", (int)Instr, (int)Reg);
        return false;
    }

    return true;
}

static bool DoPushmPopmInstr(char *Cursor, FILE *Out, const instruction Instr)
{
    assert(Cursor);
    assert(Out);

    regs Reg = UNDEF_REG;

    char RegStr[5] = "";

    if(Cursor[0] == ' ' && Cursor[1] == '[')
    {
        Cursor += 2;

        for (int i = 0; i < 4 && *Cursor > ' '; i++)
            RegStr[i] = *Cursor++;
        
        if(*Cursor > ' ')
            return true;

        if(RegStr[2] == ']')
            RegStr[2] = '\0';
        
        else if(RegStr[3] == ']')
            RegStr[3] = '\0';
        
        else
            return true;

        //printf("%s\n",RegStr);



        if(RecognizeRegAndMove(RegStr, &Reg) && !CheckEndLine(Cursor))
        {
            fprintf(Out, "%d %d ", (int)Instr, (int)Reg);
            
            return false;
        }

        return true;
    }

    return true;
}