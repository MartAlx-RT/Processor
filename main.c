//#include "Assembler.h"
//#include "../EvgeniyOnegin/Output.h"
//#include "Common.h"
//#include "StrFunctions.h"
//#include <stdio.h>

#include "Assembler.h"
#include "Processor.h"

int main(void)
{

    Assemble("output.asm", "Program.bin");

    //DisAssemble("Program.bin");

    //LablesPrint();

    // DisAssemble("Program.bin");

    
    processor p = {};

    ProcessorInit(&p, "Program.bin");

    //ProcessorDump(&p);

    processor_err_struct_t Err = {};

    // if(LaunchProgram(&p, &Err))
    //     ProcessorDump(&p, &Err);
    LaunchProgram(&p, &Err);

    ProcessorDestroy(&p);
    
    return 0;

    
}