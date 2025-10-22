#pragma once

#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "Common.h"
#include <stdio.h>
#include "Processor.h"
#include "Assembler.h"

static const size_t MAX_LABEL_LENGTH = 50;


char *AtoIAndMove(char *Instr, long long int *Num);


//char *CompareAndMove(char *CompInstr, const char *RefInstr);

char *RecognizeAndMove(char *CompInstr, instruction *Instr);

void ExcludeComments(char *Program);

//true in error case
bool CheckEndLine(char *Line);

char *RecognizeRegAndMove(char *CompReg, regs *Reg);

char *SkipSpaces(char *Cursor);

unsigned int CountInstrInLine(char *Cursor);

char *CompareAndMove(char *CompInstr, const char *RefInstr);

bool MakeLabel(char *Cursor, label *Label, size_t *NumOfLabels,
                long long int InstrCount, unsigned int LineCount, int *SameLabelIndx);

char *RecognizeLabelAndMove(char *Cursor, label *Label, size_t NumOfLabels, long long int *LabelLoc);

