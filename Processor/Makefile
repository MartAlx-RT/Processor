compile_flags = -D _DEBUG -ggdb3 -O0 -Wall -Wextra -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
linker_flags = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
compiler = g++

# compile_flags =
# linker_flags =
# compiler = gcc

all: main

main:  Common.o Assembler.o Input.o main.o Stack.o DisAssembler.o StrFunctions.o Processor.o JmpComparators.o Proc_Init_Destroy.o GraphicalInterface.o
	$(compiler) $(linker_flags) Common.o Assembler.o Input.o main.o Stack.o DisAssembler.o StrFunctions.o Processor.o JmpComparators.o Proc_Init_Destroy.o GraphicalInterface.o -lm -lncursesw

Assembler.o: Assembler.c
	$(compiler) -c $(compile_flags) Assembler.c

main.o: main.c
	$(compiler) -c $(compile_flags) main.c

Stack.o: ../Stack/Stack.c
	$(compiler) -c $(compile_flags) ../Stack/Stack.c

Input.o: ../EvgeniyOnegin/Input.c
	$(compiler) -c $(compile_flags) ../EvgeniyOnegin/Input.c

DisAssembler.o: DisAssembler.c
	$(compiler) -c $(compile_flags) DisAssembler.c

Common.o: Common.c
	$(compiler) -c $(compile_flags) Common.c

StrFunctions.o: StrFunctions.c
	$(compiler) -c $(compile_flags) StrFunctions.c

Processor.o: Processor.c
	$(compiler) -c $(compile_flags) Processor.c

# Preassembler.o: Preassembler.c
# 	$(compiler) -c $(compile_flags) Preassembler.c
# AssemblerDo.o: AssemblerDo.c
# 	$(compiler) -c $(compile_flags) AssemblerDo.c

# ProcessorDo.o: ProcessorDo.c
# 	$(compiler) -c $(compile_flags) ProcessorDo.c

JmpComparators.o: JmpComparators.c
	$(compiler) -c $(compile_flags) JmpComparators.c

Proc_Init_Destroy.o: Proc_Init_Destroy.c
	$(compiler) -c $(compile_flags) Proc_Init_Destroy.c

GraphicalInterface.o: GraphicalInterface.c
	$(compiler) -c $(compile_flags) GraphicalInterface.c

clean:
	rm *.o