#include "Assembler.h"

bool PreAssemble(const char *InFilePath, TableOfContent *CodeLine[], unsigned int *NumOfLines)
{
    if(InFilePath == NULL)
        return true;

    if(CodeLine == NULL)
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

            if(CheckEndLine(Cursor) || Lable >= 1000)
            {
                _print_err("?lable?\t");
                _print_location(InFilePath, i);

                free(Cursor);

                return true;
            }

            Lables[Lable] = (int)InstrCounter + 1;

            InstrCounter += 2;
        }

        else
            InstrCounter += CountInstrInLine(Cursor);
    }

    return false;
}