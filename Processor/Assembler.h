#pragma once

#include <stdbool.h>
#include "../EvgeniyOnegin/Input.h"
#include <string.h>
#include <stdlib.h>
#include "Common.h"
#include "WrgAndErr.h"
#include "StrFunctions.h"
#include "Processor.h"
//#include "AssemblerDo.h"


#include "ColorPrinting.h"
/*
static bool DoPush(char *Cursor, FILE *Out);
static bool DoInstrWithoutArg(char *Cursor, FILE *Out, instruction Instr);
static bool DoPushrPoprInstr(char *Cursor, FILE *Out, const instruction Instr);
static bool DoJmpInstr(char *Cursor, FILE *Out, instruction Instr, int *LabelsArr);
*/
bool Assemble(const char *InFilePath, const char *OutFilePath);
void DisAssemble(const char *ByteCodePath);


//bool PreAssemble(const char *InFilePath, TableOfContent *CodeLine[], unsigned int *NumOfLines, int *LabelsArr[]);

