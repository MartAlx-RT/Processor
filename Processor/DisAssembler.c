#include "Assembler.h"

static const char *ReverseRecognize(instruction Instr)
{
    if(Instr>=COMMANDS_SIZE)
        return COMMANDS[_UNDEF];//wrong!

    return COMMANDS[Instr];
}

static const char *ReverseRegRecognize(regs Reg)
{
    if((size_t)Reg >= NUM_OF_REGS)
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

    //char c = 0;

    int I_Instr = 0;
    unsigned int InstrCounter = 0;
    while (fscanf(ByteCode, "%d", &I_Instr) > 0)
    {
        Instr = (instruction)I_Instr;

        if (Instr >= _UNDEF)
        {
            _print_wrg("undefined instruction\n");

            //while((c=fgetc(ByteCode))!='\n' && c!=EOF);

            continue;
        }

        printf(colorize("%5u |", _CYAN_) colorize("%6s", _MAGENTA_), InstrCounter++, ReverseRecognize(Instr));

        int I_Reg = 0;
        switch (Instr)
        {
        case PUSH:

            fscanf(ByteCode, "%lld", &Arg);

            printf(colorize(" %lld", _GREEN_), Arg);

            InstrCounter++;

            break;

        case PUSHR:
        case POPR:

            fscanf(ByteCode, "%d", &I_Reg);

            Reg = (regs)I_Reg;

            printf(colorize(" %s", _GREEN_), ReverseRegRecognize(Reg));

            InstrCounter++;

            break;
        
        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
        case JMP:
        case CALL:

            fscanf(ByteCode, "%lld", &Arg);

            printf(colorize(" :%lld", _YELLOW_), Arg);

            InstrCounter++;

            break;
        
        case PUSHM:
        case POPM:
        case PUSHV:
        case POPV:

            fscanf(ByteCode, "%lld", &Arg);

            printf(colorize(" [%s]", _BLUE_), ReverseRegRecognize((regs)Arg));

            InstrCounter++;

            break;

        case OUT:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case SQRT:
        case HLT:
        case RET:
        case DRW:
        case PAUSE:
        case _SKIP_LINE:
        case _UNDEF:
        default:
            break;
        }

        putchar('\n');
    }

    fclose(ByteCode);

    printf(colorize("\n\n--------DISASSEMBLING FINISHED------\n\n", _CYAN_));
}