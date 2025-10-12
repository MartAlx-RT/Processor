#include "Assembler.h"

bool PreAssemble(const char *InFilePath, TableOfContent *CodeLine[], unsigned int *NumOfLines)
{
    //bool ErrorStatus = false;

    assert(InFilePath);
    assert(CodeLine);
    // assert(OutFilePath);

    //-------------------------------------OPEN--------------------------------


    char *Cursor = CreateStrBuffer(InFilePath);

    //printf("buffersize=%llu\n", strlen(Program));

    //FILE *Out = fopen(OutFilePath, "w");

    if(Cursor == NULL)
    {
        free(Cursor);
        //fclose(Out);


        _print_err("File(s) can't open\n");

        return true;//?
    }

    //unsigned int NumOfLines = 0;

    (*CodeLine) = CreateTOC(Cursor, NumOfLines, '\n');
    assert((*CodeLine));

    

    

    //ExcludeComments(Program);

    struct stat FileInfo = {};

    if(stat(InFilePath, &FileInfo))
    {
        _print_err("File(s) can't open\n");

        free(Cursor);

        return true;//?
    }

    

    // if(Labels)
    // {
    //     _print_err("Labels calloc error\n");

    //     return true;//?
    // }

    //-------------------------------------------------------------------------

    ExcludeComments(Cursor);

    unsigned int InstrCounter = 0;
    long long int Lable = 0;

    for (unsigned int i = 0; i < *NumOfLines; i++)
    {
        Cursor = (*CodeLine)[i].Line;

        Cursor = SkipSpaces(Cursor);

        // if (*Cursor == ';')
        // {
        //     *Cursor = '\0';

            
        // }

        /*else*/ if(*Cursor == ':')
        {
            *Cursor++ = '\0';

            Cursor = AtoIAndMove(Cursor, &Lable);

            Cursor = SkipSpaces(Cursor);

            // if (*Cursor == ';')
            // {
            //     *Cursor = '\0';

            //     //continue;
            // }

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

    //free(Cursor);

    return false;
}