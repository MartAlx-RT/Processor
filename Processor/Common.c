#include "Common.h"

const char *COMMANDS[]= {"push","out","hlt","add","sub","mul","div","sqrt", "pushr", "popr"};

const char *REGS_NAME[] =
    {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

const size_t COMMANDS_SIZE = sizeof(COMMANDS);

const size_t NUM_OF_REGS = sizeof(REGS_NAME) / sizeof(char *);//Doesn't work