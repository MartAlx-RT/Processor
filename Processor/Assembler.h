#pragma once

#include <stdbool.h>
#include "../EvgeniyOnegin/Input.h"
//#include "../EvgeniyOnegin/Output.h"
//#include "ColorPrinting.h"
#include <string.h>
//#include "../Stack/Stack.h"
#include <stdlib.h>
#include "Common.h"
#include "WrgAndErr.h"
#include "StrFunctions.h"
#include "Processor.h"


#include "ColorPrinting.h"

#define _print_err(Err) \
printf(colorize("Error\t", _RED_) colorize(Err, _YELLOW_))

#define _print_location(File, Line) \
printf(colorize("%s:%d\n",_GRAY_),File, Line+1)


#define _print_wrg(Wrg) \
printf(colorize("Warning\t", _CYAN_) colorize(Wrg, _GRAY_))

//extern const char *COMMANDS[];

bool Assemble(const char *InFilePath, const char *OutFilePath);

//void ExcludeComments(char *Program);

//int CommandCompare(const char *Command1, const char *Command2);

void DisAssemble(const char *ByteCodePath);

static bool DoPushr(char *Cursor, FILE *Out);

static bool DoPopr(char *Cursor, FILE *Out);

