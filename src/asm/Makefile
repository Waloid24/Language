CXX 		?= g++
CXXFLAGS 	?= -mavx -mavx2 -g --static-pie -std=c++14 -fmax-errors=100 \
-Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat \
-Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual -Wchar-subscripts \
-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body \
-Wfloat-equal 	-Wformat-nonliteral -Wformat-security -Wformat-signedness \
-Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith \
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types \
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros \
-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -fcheck-new -fsized-deallocation \
-fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -fPIE -fsanitize=address -fsanitize=alignment \
-fsanitize=bool -fsanitize=bounds -fsanitize=enum \
-fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero \
-fsanitize=integer-divide-by-zero -fsanitize=leak \
-fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size \
-fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift \
-fsanitize=signed-integer-overflow -fsanitize=undefined \
-fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie

SRC_ASM		:= mainAsm.cpp asm.cpp
OUT_ASM		:= asm.o

SRC_CPU		:= mainCpu.cpp cpu.cpp stack.cpp
OUT_CPU		:= cpu.o

SRC_COMMON	:= readCode.cpp
SRC_DEBUG	:= debug.cpp
SRC_STRING	:= myStrcmp.cpp

PREF_COMMON	:= ../common/
PREF_DEBUG	:= ../debug/
PREF_STRING	:= ../string/
VPATH		:= $(PREF_COMMON) $(PREF_DEBUG) $(PREF_STRING)

OBJ_ASM		:= $(addprefix $(OBJDIR)/, $(SRC_ASM:.cpp=.tmp.o) $(SRC_COMMON:.cpp=.tmp.o) $(SRC_DEBUG:.cpp=.tmp.o) $(SRC_STRING:.cpp=.tmp.o))

OBJ_CPU		:= $(addprefix $(OBJDIR)/, $(SRC_CPU:.cpp=.tmp.o) $(SRC_COMMON:.cpp=.tmp.o) $(SRC_DEBUG:.cpp=.tmp.o) $(SRC_STRING:.cpp=.tmp.o))

OUT_ASM		:= $(OBJDIR)/$(OUT_ASM)
OUT_CPU		:= $(OBJDIR)/$(OUT_CPU)

.PHONY: asm cpu
asm: $(OUT_ASM)
cpu: $(OUT_CPU)

$(OUT_ASM): $(OBJ_ASM)
	ld -r $(OBJ_ASM) -o $(OUT_ASM)

$(OUT_CPU): $(OBJ_CPU) | $(LOGCALLDIR)
	ld -r $(OBJ_CPU) -o $(OUT_CPU)

$(OBJDIR)/%.tmp.o: %.cpp | $(OBJDIR)
	@ $(CXX) -c $^ -o $@ $(CXXFLAGS)
