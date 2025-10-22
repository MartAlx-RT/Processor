#pragma once

#include <stdbool.h>
#include "../EvgeniyOnegin/Input.h"
#include <string.h>
#include <stdlib.h>
#include "Common.h"
#include "WrgAndErr.h"
#include "StrFunctions.h"
#include "Processor.h"
#include "ColorPrinting.h"




bool Assemble(const char *InFilePath, const char *OutFilePath);
void DisAssemble(const char *ByteCodePath);

