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

SRC		:= frontend.cpp lexer.cpp simplify.cpp textTree.cpp parser.cpp
SRC_TREE	:= tree.cpp graphDumpTree.cpp
SRC_COMMON	:= readCode.cpp
OUT		:= frontend

#Is needed in order to access the fыiles
PREF_TREE	:= ../tree/
PREF_COMMON	:= ../common/
PREF_DEBUG	:= ../debug/
VPATH 		:= $(PREF_TREE) $(PREF_COMMON) $(PREF_DEBUG)
#end this block

OBJDIR 		:= ../obj
DESTDIR		:= ../bin
OBJ 		:= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o) $(SRC_TREE:.cpp=.o) $(SRC_COMMON:.cpp=.o))

OUT		:= $(DESTDIR)/$(OUT)

.PHONY:	all clean
all: $(OUT)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR) $(DESTDIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OUT): $(OBJ) | $(OBJDIR) $(DESTDIR)
	ld -r $(OBJ) -o $(OUT)

clean:
	@ echo ------ CLEAN $(OBJDIR) ------
	rm -f $(OUT) $(OBJ)

$(DESTDIR):
	mkdir $(DESTDIR)
$(OBJDIR):
	mkdir $(OBJDIR)
