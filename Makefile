export CXX 		:= g++
export CXXFLAGS 	:= -mavx -mavx2 -g --static-pie -std=c++14 -fmax-errors=100 \
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

export DESTDIR		:= $(CURDIR)/bin
export OBJDIR		:= $(CURDIR)/obj
export LOGDIR		:= $(CURDIR)/logs
export LOGCALLDIR	:= $(LOGDIR)/logsStack
export LOGASMDIR	:= $(LOGDIR)/logsAsm
export LOGCPUDIR 	:= $(LOGDIR)/logsCpu
export LOGTREEDIR	:= $(LOGDIR)/logsTree

.PHONY: compile frontend backend asm cpu clean
compile: frontend backend asm cpu

frontend: | $(OBJDIR) $(DESTDIR) $(LOGDIR) $(LOGTREEDIR)
	@ echo ------ COMPILE frontend ------
	@ cd $(CURDIR)/src/frontend && $(MAKE)
	@ $(CXX) $(OBJDIR)/frontend.o -o $(DESTDIR)/frontend $(CXXFLAGS)

backend: | $(OBJDIR) $(DESTDIR) $(LOGTREEDIR)
	@ echo ------ COMPILE backend ------
	@ cd $(CURDIR)/src/backend && $(MAKE)
	@ $(CXX) $(OBJDIR)/backend.o -o $(DESTDIR)/backend $(CXXFLAGS)

asm: | $(OBJDIR) $(DESTDIR) $(LOGASMDIR)
	@ echo ------ COMPILE asm ------
	@ cd $(CURDIR)/src/asm && $(MAKE) asm
	@ $(CXX) $(OBJDIR)/asm.o -o $(DESTDIR)/asm $(CXXFLAGS)

cpu: | $(OBJDIR) $(DESTDIR) $(LOGCALLDIR) $(LOGCPUDIR)
	@ echo ------ COMPILE cpu ------
	@ cd $(CURDIR)/src/asm && $(MAKE) cpu
	@ $(CXX) $(OBJDIR)/cpu.o -o $(DESTDIR)/cpu $(CXXFLAGS)

.PHONY: run quadraticEquation.txt
run:
	./bin/frontend --- $(filter-out $@,$(MAKECMDGOALS)) --- frontResult.txt
	./bin/backend --- frontResult.txt --- backResult.txt
	./bin/asm --- backResult.txt --- asmResult.bin
	./bin/cpu --- asmResult.bin

clean:
	rm -r $(DESTDIR) $(OBJDIR) $(LOGDIR)

$(DESTDIR):
	mkdir $(DESTDIR)
$(OBJDIR):
	mkdir $(OBJDIR)
$(LOGDIR):
	mkdir $(LOGDIR)
$(LOGCALLDIR):
	mkdir $(LOGCALLDIR)
$(LOGASMDIR):
	mkdir $(LOGASMDIR)
$(LOGCPUDIR):
	mkdir $(LOGCPUDIR)
$(LOGTREEDIR):
	mkdir $(LOGTREEDIR)
