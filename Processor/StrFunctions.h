#pragma once

#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "Common.h"
#include <stdio.h>
#include "Processor.h"


char *AtoIAndMove(char *Instr, long long int *Num);


//char *CompareAndMove(char *CompInstr, const char *RefInstr);

char *RecognizeAndMove(char *CompInstr, instruction *Instr);

void ExcludeComments(char *Program);

//true in error case
bool CheckEndLine(char *Line);

char *RecognizeRegAndMove(char *CompReg, regs *Reg);

