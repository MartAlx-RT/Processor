#include "Assembler.h"

static const char *ReverseRecognize(instruction Instr)
{
    if(Instr>=COMMANDS_SIZE/sizeof(char*))
        return COMMANDS[_UNDEF];

    return COMMANDS[Instr];
}

static const char *ReverseRegRecognize(regs Reg)
{
    if(Reg >= NUM_OF_REGS)
        return REGS_NAME[UNDEF_REG];

    return REGS_NAME[Reg];
}

void DisAssemble(const char *ByteCodePath)
{

    if (ByteCodePath == NULL)
    {
        _print_err("Bytecode file(s) can't open\n");
        return;
    }

    FILE *ByteCode = fopen(ByteCodePath, "r");

    if (ByteCode == NULL)
    {
        _print_err("Bytecode file(s) can't open\n");
        return;
    }

    printf(colorize("\n\n-----DISASSEMBLING IS IN PROGRESS-----\n\n", _CYAN_));

    long long Arg = 0;

    regs Reg = UNDEF_REG;

    instruction Instr = _UNDEF;

    char c = 0;

    while(fscanf(ByteCode,"%d", &Instr) > 0)
    {
        if(Instr>=_UNDEF)
        {
            _print_wrg("undefined instruction\n");

            //while((c=fgetc(ByteCode))!='\n' && c!=EOF);

            continue;
        }

        printf(colorize("%s", _MAGENTA_), ReverseRecognize(Instr));

        switch(Instr)
        {
        case PUSH:

            fscanf(ByteCode, "%lld", &Arg);

            printf(colorize(" %lld", _GREEN_), Arg);

            break;

        case PUSHR:

            fscanf(ByteCode, "%d", &Reg);

            printf(colorize(" %s", _GREEN_), ReverseRegRecognize(Reg));

            break;
        
        case POPR:

            fscanf(ByteCode, "%d", &Reg);

            printf(colorize(" %s", _GREEN_), ReverseRegRecognize(Reg));

            break;
        }

        putchar('\n');
    }

    fclose(ByteCode);

    printf(colorize("\n\n--------DISASSEMBLING FINISHED------\n\n", _CYAN_));
}